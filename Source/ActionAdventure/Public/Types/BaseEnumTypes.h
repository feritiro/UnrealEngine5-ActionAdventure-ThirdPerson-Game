#pragma once

UENUM()
enum class EBaseConfirmType : uint8
{
    Yes,
    No
};

UENUM()
enum class EBaseValidType : uint8
{
    Valid,
    Invalid
};

UENUM()
enum class EBaseSuccessType : uint8
{
    Successful,
    Failed
};