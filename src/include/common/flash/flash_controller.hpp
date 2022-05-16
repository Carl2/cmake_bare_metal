#pragma once
#include <optional>
#include <ranges>
#include "flash_def.hpp"
#include "main.h"

namespace flash
{

using maybe_error = std::optional<uint32_t>;
maybe_error erasePage(unsigned long page_addr);

/**
 *  \brief Write data to flash
 *
 *  This writes data to flash based on the PAGE_ADDR
 *  If the PAGE_ADDR is not included in the template argument,
 *  it will use the stm32f103t8u7 last Page address (32 Kb).
 *
 *  \param Address in which to write data to.
 *  \param buffer - std::array<uint32_t,N> where N is defined
 *  \return maybe_error - if false, then no error...
 */
template <size_t N, uint32_t PAGE_ADDR = FLASH_PAGE>
maybe_error writeData(unsigned long addr, std::array<uint32_t, N> const& buffer)
{
    HAL_FLASH_Unlock();

    erasePage(PAGE_ADDR);  // defined by us and most likely same as the addr variable

    /* Program the user Flash area word by word*/
    for (auto& slot : buffer)
    {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr, slot) == HAL_OK)
        {
            addr += 4;
        }
        else
        {
            /* Error occurred while writing data in Flash memory*/
            return {HAL_FLASH_GetError()};
        }
    }

    /* Lock the Flash to disable the flash control register access (recommended
        to protect the FLASH memory against possible unwanted operation) *********/
    HAL_FLASH_Lock();

    return std::nullopt;
}

// void readData(uint32_t addr, auto& buffer);

template <size_t N>
std::array<uint32_t, N> readData(uint32_t addr)
{
    std::array<uint32_t, N> buffer{};

    for (auto& slot : buffer)
    {
        slot = *reinterpret_cast<__IO uint32_t*>(addr);
        addr += sizeof(uint32_t);
    }

    return buffer;
}

}  // namespace flash
