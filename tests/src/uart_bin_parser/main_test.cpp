#include <ranges>
#include <string_view>
#include <fmt/format.h>
#include <gtest/gtest.h>
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
        ASSERT_EQ(cb.recv_data.sz, 8);
        auto cb_iter = cb.recv_data.data.begin();
        for (const auto& item : payload | std::ranges::views::take(8))
        {
            fmt::print("{} == {}\n", item, *cb_iter);
            // ASSERT_EQ(*cb_iter, item);
            ++cb_iter;
        }
    }
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    // InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
