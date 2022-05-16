#include "flash_controller.hpp"
#include <array>
#include "flash_def.hpp"

namespace flash
{

maybe_error erasePage(unsigned long page_addr)
{
    static FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t PAGEError;

    /* Fill EraseInit structure*/
    EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.PageAddress = page_addr;
    EraseInitStruct.NbPages     = 1;

    /* Erase the user Flash area*/
    if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)
    {
        /*Error occurred while page erase.*/
        return {HAL_FLASH_GetError()};
    }

    return {};
}

}  // namespace flash
