#include <ranges>
#include <string_view>
#include <fmt/format.h>
#include <gtest/gtest.h>
#include "crc16_calc.hpp"
#include "msg_def.hpp"
#include "msg_sml.hpp"
#include "uart_bin_fixture.hpp"
//#include "motor/motor_hw.hpp"
#include <bit>
#include "bin_parser.hpp"
/*
 using ::testing::_;
 using ::testing::Return;
 using ::testing::SetArgReferee;
 using ::testing::Throw;
 using ::testing::SaveArg;
 using ::testing::StartsWith;
*/

TEST_F(Uart_comm_test, header_check)
{
    if constexpr (std::endian::native == std::endian::big)
    {
        fmt::print("Big endian\n");
    }
    else
    {
        fmt::print("Little endian\n");
    }
    uint8_t len = 0x06;
    msg::Uart_buffer_t msg{0xaa, 0xaa, 0x00, 0x01, 0x00, len};
    m_sm.new_message(msg, msg.size());

    ASSERT_EQ(m_sm.ctx_.hdr.id, 0xaaaa);
    ASSERT_EQ(m_sm.ctx_.hdr.cmd, 0x0001);
    ASSERT_EQ(m_sm.ctx_.hdr.len, 0x0006);
    ASSERT_EQ(cb.recv_irq_sz, len + 2);  // Len + CRC

    {
        msg::Uart_buffer_t payload = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0xf0, 0xc4};
        m_sm.new_message(payload, len + 2);
        ASSERT_EQ(cb.recv_irq_sz, msg::HDR_SZ);  // Check reset to wait for Header
        ASSERT_EQ(cb.recv_data.data_span.size(), 8);
        ASSERT_EQ(cb.address, 0xaaaa);
        ASSERT_EQ(cb.recv_data.hdr.id, 0xaaaa);
        ASSERT_EQ(cb.recv_data.hdr.cmd, 0x0001);
        ASSERT_EQ(cb.recv_data.hdr.len, 0x0006);
        auto cb_iter = cb.recv_data.data_span.begin();
        for (const auto& item : payload | std::ranges::views::take(8))
        {
            fmt::print("{} == {}\n", item, *cb_iter);
            // ASSERT_EQ(*cb_iter, item);
            ++cb_iter;
        }
    }
}

TEST_F(Uart_comm_test, check_address_test)
{
    cb                   = {};
    cb.ret_address_check = msg::AddressMode::BROADCAST;  // BROADCAST message
    msg::Uart_buffer_t msg{0xaa, 0xaa, 0x00, 0x01, 0x00, 0x06};
    m_sm.new_message(msg, msg::HDR_SZ);
    ASSERT_EQ(cb.address, 0);  // to show that address check happens not when the header is received, but when the body arrives

    {
        msg::Uart_buffer_t payload = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0xf0, 0xc4};
        m_sm.new_message(payload, 8);  // len(body) + crc
        ASSERT_EQ(cb.address, 0xaaaa); // Now we have the address (see comment above)
        ASSERT_EQ(cb.recv_data.data_span.size(), 8);
        ASSERT_EQ(cb.recv_irq_sz, msg::HDR_SZ);  // Check reset to wait for Header
    }

    cb = {};

    {
        msg::Uart_buffer_t msg2{0x00, 0x01, 0x00, 0x01, 0x00, 0x01};
        cb.ret_address_check =
            msg::AddressMode::NOT_APPLICABALE;  // NOT_APPLICABLE meaning we should ignore it.
        m_sm.new_message(msg2, msg2.size());
        ASSERT_EQ(cb.recv_irq_sz, 3);  // 1 byte + 2 byte CRC
    }

    {
        msg::Uart_buffer_t payload2{0x01, 0xa1, 0xa2};
        m_sm.new_message(payload2, payload2.size());
        ASSERT_EQ(cb.recv_data.data_span.size(), 0);  // No callback since check
                                                      // address returned false,
                                                      // we ignore the message.
        ASSERT_EQ(cb.recv_data.hdr.id, 0);
        ASSERT_EQ(cb.recv_data.hdr.cmd, 0);
        ASSERT_EQ(cb.recv_data.hdr.len, 0);
        ASSERT_TRUE(m_sm.is_wait4header());
        ASSERT_EQ(cb.recv_irq_sz, msg::HDR_SZ);  // Check reset to wait for Header
    }
    cb = {};
    {
        msg::Uart_buffer_t msg2{0x00, 0x01, 0x00, 0x01, 0x00, 0x01};
        cb.ret_address_check = msg::AddressMode::TO_THIS;  // To_this printhead
        m_sm.new_message(msg2, msg2.size());
        ASSERT_EQ(cb.recv_irq_sz, 3);  // 1 byte + 2 byte CRC
    }
}

TEST_F(Uart_comm_test, test_crc)
{  // clang-format off
    cb                   = {};
    msg::Uart_buffer_t hdr_msg = {
        0xaa, 0xaa,  // Id
        0x00,0x01,  // Cmd
        0x00,0x06}; // Len "0x00 06"
    msg::Uart_buffer_t pay_msg = {
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, // Payload
        0xf0, 0xc4};  //CRC
    // clang-format on
    // Send  the header
    {
        m_sm.new_message(hdr_msg, 6);
        ASSERT_EQ(cb.recv_irq_sz, 6 + 2);  // Len + CRC
        ASSERT_EQ(m_sm.ctx_.hdr.len, 0x0006);
    }
    // Send the payload
    {
        m_sm.new_message(pay_msg, 8);
        ASSERT_EQ(cb.recv_irq_sz, 6);  // Header length
        ASSERT_EQ(cb.recv_data.hdr.id, 0xaaaa);
        ASSERT_EQ(cb.recv_data.hdr.cmd, 1);
        ASSERT_EQ(cb.recv_data.hdr.len, 6);
        // Check payload
    }
}

TEST_F(Uart_comm_test, test_no_payload)
{  // clang-format off
    cb                   = {};
    msg::Uart_buffer_t hdr_msg = {
        0x00, 0x01,  // Id
        0x00,0x04,  // Cmd
        0x00,0x00}; // Len "0x00 00"
    msg::Uart_buffer_t pay_msg = { // Payload
        0x23, 0xb8};  //CRC
    // clang-format on
    // Send  the header
    {
        m_sm.new_message(hdr_msg, 6);
        ASSERT_EQ(cb.recv_irq_sz, 0 + 2);  // Len + CRC
        ASSERT_EQ(m_sm.ctx_.hdr.len, 0x0000);
    }
    // Send the payload
    {
        m_sm.new_message(pay_msg, 2);
        ASSERT_EQ(cb.recv_irq_sz, 6);  // Header length
        ASSERT_EQ(cb.recv_data.hdr.id, 0x0001);
        ASSERT_EQ(cb.recv_data.hdr.cmd, 0x0004);
        ASSERT_EQ(cb.recv_data.hdr.len, 0x0000);
        // Check payload
    }
}

TEST_F(Uart_comm_test, test_timeout)
{  // clang-format off
    cb                   = {};
    msg::Uart_buffer_t hdr_msg = {
        0xaa, 0xaa,  // Id
        0x00,0x01,  // Cmd
        0x00,0x06}; // Len "0x00 06"
    msg::Uart_buffer_t pay_msg = {
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, // Payload
        0xf0, 0xc4};  //CRC
    // clang-format on
    // Send  the header
    {
        m_sm.new_message(hdr_msg, 6);
        ASSERT_EQ(cb.recv_irq_sz, 6 + 2);  // Len + CRC
        ASSERT_EQ(m_sm.ctx_.hdr.len, 0x0006);
    }

    // Timeout
    {
        m_sm.timeout();
        ASSERT_TRUE(m_sm.is_wait4header());
        ASSERT_EQ(cb.recv_irq_sz, msg::HDR_SZ);  // Check reset to wait for Header
    }

    // Send  the header
    {
        m_sm.new_message(hdr_msg, 6);
        ASSERT_EQ(cb.recv_irq_sz, 6 + 2);  // Len + CRC
        ASSERT_EQ(m_sm.ctx_.hdr.len, 0x0006);
    }

    ASSERT_TRUE(m_sm.is_wait4msg());

    // Send the payload
    {
        m_sm.new_message(pay_msg, 8);
        ASSERT_EQ(cb.recv_irq_sz, 6);  // Header length
        ASSERT_EQ(cb.recv_data.hdr.id, 0xaaaa);
        ASSERT_EQ(cb.recv_data.hdr.cmd, 1);
        ASSERT_EQ(cb.recv_data.hdr.len, 6);
        // Check payload
    }
}

TEST_F(Uart_comm_test, test_timeout2)
{  // clang-format off
    cb                   = {};
    msg::Uart_buffer_t hdr_msg = {
        0xaa, 0xaa,  // Id
        0x00,0x01,  // Cmd
        0x00,0x08}; // Len "0x00 08"
    msg::Uart_buffer_t pay_msg = {
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, // Payload only 7 bytes
        0xf0, 0xc4};  //CRC
    // clang-format on
    // Send  the header
    {
        m_sm.new_message(hdr_msg, 8);
        ASSERT_EQ(cb.recv_irq_sz, 8 + 2);  // Len + CRC
        ASSERT_EQ(m_sm.ctx_.hdr.len, 0x0008);
    }

    // Send the short payload and timeout
    {
        m_sm.new_message(pay_msg, 7+2);
        m_sm.timeout();
        ASSERT_TRUE(m_sm.is_wait4header());
    }

    // We can receive the header again
    {
        m_sm.new_message(hdr_msg, 8);
        ASSERT_EQ(cb.recv_irq_sz, 8 + 2);  // Len + CRC
        ASSERT_EQ(m_sm.ctx_.hdr.len, 0x0008);
        ASSERT_TRUE(m_sm.is_wait4msg());
    }
}


int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    // InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
