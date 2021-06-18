//------------------------------------------------------------------------------
// Copyright (C) 2020 The BeeTree Authors - All Rights Reserved
//
// This file is subject to the terms and conditions defined in the
// file 'LICENSE', which is part of this source code package.
//------------------------------------------------------------------------------

#include <array>
#include <future>
#include <nlohmann/json.hpp>

#include "beetree/experimental/expected.hpp"
#include "internal-platform.hpp"

namespace helloworld {

// for convenience
using json = nlohmann::json;

void trace(const std::string &type, const std::string &name, bool value)
{
    std::cout << type << ": " << name << " " << (value ? "ON" : "OFF") << std::endl;
}

void InternalPlatform::DiscreteOutput::set()
{
    this->value = true;
    trace("DO", this->name, this->value);
}
void InternalPlatform::DiscreteOutput::clear()
{
    this->value = false;
    trace("DO", this->name, this->value);
}
void InternalPlatform::DiscreteOutput::toggle()
{
    this->value = !this->value;
    trace("DO", this->name, this->value);
}
bool InternalPlatform::DiscreteOutput::is_set() const { return this->value; }

bool InternalPlatform::SerialDevice::open(const char *host, uint16_t port)
{
    return udp.open(host, port);
}

int32_t InternalPlatform::SerialDevice::write(std::size_t              address,
                                              const uint8_t *          data,
                                              std::size_t              len,
                                              bte::chrono::duration_ms timeout)
{
    if (udp.send(data, len))
    {
        return len;
    }
    return 0;
}
int32_t InternalPlatform::SerialDevice::read(std::size_t              address,
                                             uint8_t *                data,
                                             std::size_t              len,
                                             bte::chrono::duration_ms timeout)
{
    return 0;
}

void InternalPlatform::SerialDevice::close() { udp.close(); }

InternalPlatform &InternalPlatform::instance()
{
    static InternalPlatform s_instance;
    return s_instance;
}

struct boot_api
{
    struct error
    {
        constexpr error() = default;
    };    
    
    using expected = bte_experimental::expected<void,error>;
    using unexpected = bte_experimental::unexpected<error>;

    static expected open_vcp_tx(InternalPlatform &ip)
    {
        if (!ip.vcp.open(ip.local_host(), ip.TX_PORT))
        {
            return unexpected(error{});
        }
        return expected();
    }

    static expected open_udp_rx(InternalPlatform &ip)
    {
        if (!ip.m_udpRX.open(ip.RX_PORT))
        {
            return unexpected(error{});
        }
        return expected();
    }

    static expected start_thread(InternalPlatform &ip)
    {
        auto p = std::promise<void>{};
        auto f = p.get_future();

        ip.m_thread = std::thread(
            [&](std::promise<void> p) {
                ip.m_is_running = true;
                p.set_value();
                ip.run_coms();
            },
            std::move(p));

        if (std::future_status::ready != f.wait_for(std::chrono::seconds{1}))
        {
            return unexpected(error{});
        }
        return expected();
    }
};

bool InternalPlatform::boot()
{
    // ensure the platform is in a good state ... so tear it down
    teardown();

    // 1. open a vcp/serial port on the local host to transmit
    auto expected = boot_api::open_vcp_tx(*this)
                        // 2. open udp receiving port
                        .and_then([&]() { return boot_api::open_udp_rx(*this); })
                        // 3. start the receiving thread
                        .and_then([&]() { return boot_api::start_thread(*this); });

    // teardown if something unexpected happened
    if (!expected)
    {
        teardown();
    };

    return expected;
}

void InternalPlatform::run_coms()
{
    constexpr std::chrono::microseconds TIMEOUT = std::chrono::milliseconds{10};
    std::array<uint8_t, 512>            buffer;

    while (m_is_running)
    {
        auto nbytes = m_udpRX.recv(buffer.begin(), buffer.size(), TIMEOUT);
        if (nbytes > 0)
        {
            auto msg = json::parse(buffer.begin(), buffer.begin() + nbytes);
            for (const auto &it : msg.items())
            {
                const auto &key   = it.key();
                const auto &value = it.value();
                if (key == "user_btn_depressed")
                {
                    this->user_btn.set(value.get<bool>());
                }
            }
        }
    }
}

void InternalPlatform::teardown()
{
    m_is_running = false;
    if (m_thread.joinable())
    {
        m_thread.join();
    }

    m_udpRX.close();
    vcp.close();
}

bool boot(Platform &) { return InternalPlatform::instance().boot(); }
void teardown(Platform &) { InternalPlatform::instance().teardown(); }

// > Clock
bte::ISystemClock &Platform::clock() { return InternalPlatform::instance().clock; }

// > Discrete Outputs
bte::IDiscreteOutput &Platform::led() { return InternalPlatform::instance().led; }
bte::IDiscreteOutput &Platform::debug() { return InternalPlatform::instance().debug; }

// > Discrete Inputs
bte::IDiscreteInput &Platform::user_btn() { return InternalPlatform::instance().user_btn; }

bte::ISerialDevice &Platform::vcp() { return InternalPlatform::instance().vcp; }
}  // namespace helloworld