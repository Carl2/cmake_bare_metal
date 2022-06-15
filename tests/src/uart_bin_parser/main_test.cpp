#include <ranges>
#include <string_view>
#include <fmt/format.h>
#include <gtest/gtest.h>
#include "crc16_calc.hpp"
#include "msg_def.hpp"
#include "msg_sml.hpp"
#include "uart_bin_fixture.hpp"
//#include "motor/motor_hw.hpp"
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
    // Little endian
    msg::Uart_buffer_t msg{0xaa, 0xaa, 0x01, 0x00, 0x06, 0x00};
    m_sm.new_message(msg, msg.size());

    ASSERT_EQ(m_sm.ctx_.hdr.id, 0xaaaa);
    ASSERT_EQ(m_sm.ctx_.hdr.cmd, 0x0001);
    ASSERT_EQ(m_sm.ctx_.hdr.len, 0x0006);
    ASSERT_EQ(cb.recv_irq_sz, 6 + 2);  // Len + CRC

    {
        msg::Uart_buffer_t payload = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0xff, 0xff};
        m_sm.new_message(payload, 8);
        ASSERT_EQ(cb.recv_irq_sz, 6);  // Set to wait for Header size = 6
        ASSERT_EQ(cb.recv_data.data_span.size(), 8);
        ASSERT_EQ(cb.address, 0xaaaa);
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
    cb.ret_address_check = true;  // Its true by default..
    msg::Uart_buffer_t msg{0xaa, 0xaa, 0x01, 0x00, 0x06, 0x00};
    m_sm.new_message(msg, msg.size());
    ASSERT_EQ(cb.address, 0);

    {
        msg::Uart_buffer_t payload = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0xff, 0xff};
        m_sm.new_message(payload, 8);
        ASSERT_EQ(cb.recv_irq_sz, 6);  // Set to wait for Header size = 6
        ASSERT_EQ(cb.address, 0xaaaa);
        ASSERT_EQ(cb.recv_data.data_span.size(), 8);
    }

    cb.recv_data.data_span = {};

    {
        msg::Uart_buffer_t msg2{0x01, 0x00, 0x01, 0x00, 0x01, 0x00};
        cb.ret_address_check = false;  // Now returning false, meaning we should ignore it.
        m_sm.new_message(msg2, msg2.size());
        ASSERT_EQ(cb.recv_irq_sz, 3);  // 1 byte + 2 byte CRC
    }
    {
        msg::Uart_buffer_t payload2{0x01, 0xa1, 0xa2};
        m_sm.new_message(payload2, payload2.size());
        ASSERT_EQ(cb.recv_data.data_span.size(), 0);  // No callback since check
                                                      // address returned false,
                                                      // we ignore the message.
    }
}

TEST_F(Uart_comm_test, test_crc)
{  // clang-format off
    cb                   = {};
    msg::Uart_buffer_t hdr_msg = {
        0xaa, 0xaa,  // Id
        0x01, 0x00,  // Cmd
        0x06, 0x00}; // Len
    msg::Uart_buffer_t pay_msg = {
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, // Payload
        0x8c,0xed};  //CRC
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
    }
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    // InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
