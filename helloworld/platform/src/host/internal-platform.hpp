//------------------------------------------------------------------------------
// Copyright (C) 2020 The BeeTree Authors - All Rights Reserved
//
// This file is subject to the terms and conditions defined in the
// file 'LICENSE', which is part of this source code package.
//------------------------------------------------------------------------------
#pragma once

#include <atomic>
#include <iostream>
#include <string>
#include <thread>
#include "beetree/platform/os/system-clock.hpp"
#include "beetree/platform/os/udp-receive.hpp"
#include "beetree/platform/os/udp-send.hpp"
#include "helloworld-platform/helloworld-platform.hpp"

namespace helloworld {
class InternalPlatform
{
public:
    using SystemClock = host::SystemClock;

    /// The internal platform's Serial Device sends serial writes
    /// as UDP messages to the endpoint ipv4 127.0.0.1, port 1225
    class SerialDevice : public bte::ISerialDevice
    {
    public:
        bool    open(const char* host, uint16_t port);
        void    close();
        int32_t write(std::size_t              address,
                      const uint8_t*           data,
                      std::size_t              len,
                      bte::chrono::duration_ms timeout) override;
        int32_t read(std::size_t              address,
                     uint8_t*                 data,
                     std::size_t              len,
                     bte::chrono::duration_ms timeout) override;

    private:
        host::UDPSend udp;
    };

    class DiscreteOutputController : public bte::IRegistryTxController
    {
    public:
        DiscreteOutputController() = default;
        void flush() override;

        DiscreteOutputRegistry registry;
    };

    class DiscreteInputController : public bte::IRegistryRxController
    {
    public:
        DiscreteInputController() = default;
        
        // do nothing in this platform implementation
        // the platform updates d_ins via another thread
        void sync() override { /* do nothing */ }

        DiscreteInputRegistry registry;
    };

    static InternalPlatform& instance();
    bool                     boot();
    void                     teardown();

    // > MEMBERS

    SystemClock              clock;
    SerialDevice             vcp;
    DiscreteOutputController discrete_output_controller;
    DiscreteInputController  discrete_input_controller;

    static constexpr const char* local_host() { return "127.0.0.1"; }
    static constexpr uint16_t    TX_PORT{1225};  // like christmas
    static constexpr uint16_t    RX_PORT{TX_PORT + 1};

private:
    InternalPlatform() = default;

    friend struct boot_api;

    void run_coms();

    std::thread      m_thread;
    host::UDPReceive m_udpRX;
    std::atomic_bool m_is_running{false};
};
}  // namespace helloworld