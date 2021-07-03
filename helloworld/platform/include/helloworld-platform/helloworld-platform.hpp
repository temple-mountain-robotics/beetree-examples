///-----------------------------------------------------------------------------
/// Copyright (C) 2020 The BeeTree Authors - All Rights Reserved
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE', which is part of this source code package.
///-----------------------------------------------------------------------------

#pragma once

#include "beetree/platform/basics/array-ref.hpp"
#include "beetree/platform/basics/register-value.hpp"
#include "beetree/platform/i-registry-controllers.hpp"
#include "beetree/platform/i-serial-device.hpp"
#include "beetree/platform/i-system-clock.hpp"

namespace helloworld {
// forward declaration
class Platform;
bool boot(Platform&);
void teardown(Platform&);

enum class DiscreteInputValueKey
{
    USER_BTN_DEPRESSED = 0,
    NUM_VALUES
};

enum class DiscreteOutputValueKey
{
    LED = 0,
    DEBUG_PIN,
    NUM_VALUES
};

template <class T>
constexpr std::size_t len()
{
    return static_cast<std::size_t>(T::NUM_VALUES);
}

class DiscreteOutputRegistry
{
public:
    struct Meta
    {
        static constexpr std::size_t VERSION{0};
        static constexpr std::size_t DISCRETE_OUTPUT_BANK_BEGIN_IDX{0};
        static constexpr std::size_t DISCRETE_OUTPUT_BANK_LEN{len<DiscreteOutputValueKey>()};

        // total number of banks represented
        static constexpr std::size_t NUM_BANKS{1};
        // total number of values in the registry over all banks
        static constexpr std::size_t NUM_VALUES{DISCRETE_OUTPUT_BANK_LEN};
    };

    // direct access to the contiguous memory block
    bte::array_ref<bte::RegisterValue> data() { return m_data; }

    // virtual access to the discrete ouput register bank
    bte::array_ref<bte::RegisterValue> discrete_output_bank()
    {
        return {&m_data[Meta::DISCRETE_OUTPUT_BANK_BEGIN_IDX], Meta::DISCRETE_OUTPUT_BANK_LEN};
    }

private:
    std::array<bte::RegisterValue, Meta::NUM_VALUES> m_data;
};

class DiscreteInputRegistry
{
public:
    struct Meta
    {
        static constexpr std::size_t VERSION{0};

        static constexpr std::size_t DISCRETE_INPUT_BANK_BEGIN_IDX{0};
        static constexpr std::size_t DISCRETE_INPUT_BANK_LEN{len<DiscreteInputValueKey>()};

        // total number of banks represented
        static constexpr std::size_t NUM_BANKS{1};
        // total number of values in the registry over all banks
        static constexpr std::size_t NUM_VALUES{DISCRETE_INPUT_BANK_LEN};
    };

    // direct access to the contiguous memory block
    bte::array_ref<bte::RegisterValue> data() { return m_data; }

    // virtual access to the discrete input register bank
    bte::array_ref<bte::RegisterValue> discrete_input_bank()
    {
        return {&m_data[Meta::DISCRETE_INPUT_BANK_BEGIN_IDX], Meta::DISCRETE_INPUT_BANK_LEN};
    }

private:
    std::array<bte::RegisterValue, Meta::NUM_VALUES> m_data;
};

class Platform
{
public:
    Platform() = default;

    // > Clock
    bte::ISystemClock& clock();

    // > Registry
    bte::IRegistryTxController& discrete_outputs_controller();
    bte::IRegistryRxController& discrete_inputs_controller();
    DiscreteInputRegistry& discrete_inputs();
    DiscreteOutputRegistry& discrete_outputs();

    // > Serial Devices
    bte::ISerialDevice& vcp();
};
}  // namespace helloworld