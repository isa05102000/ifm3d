/*
 * Copyright 2021-present ifm electronic, gmbh
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ifm3d/device/o3r.h>
#include <o3r_impl.hpp>

//================================================
// O3R class - the public interface
//================================================

ifm3d::O3R::O3R(const std::string& ip,
                            const std::uint16_t xmlrpc_port)
  : ifm3d::Device::Device(ip, xmlrpc_port),
    pImpl(new ifm3d::O3R::Impl(XWrapper()))
{}

ifm3d::O3R::~O3R() = default;

void
ifm3d::O3R::FactoryReset(bool keepNetworkSettings)
{
  this->pImpl->FactoryReset(keepNetworkSettings);
}

json
ifm3d::O3R::Get(const std::vector<std::string>& path)
{
  return this->pImpl->Get(path);
}

json
ifm3d::O3R::ResolveConfig(const json::json_pointer& ptr)
{
  return this->pImpl->ResolveConfig(ptr);
}

void
ifm3d::O3R::Set(const json& j)
{
  return this->pImpl->Set(j.dump());
}

json
ifm3d::O3R::GetInit()
{
  return this->pImpl->GetInit();
}

void
ifm3d::O3R::SaveInit()
{
  return this->pImpl->SaveInit();
}

std::string
ifm3d::O3R::GetInitStatus()
{
  return this->pImpl->GetInitStatus();
}

json
ifm3d::O3R::GetSchema()
{
  return json::parse(this->pImpl->GetSchema());
}

void
ifm3d::O3R::Lock(const std::string& password)
{
  return this->pImpl->Lock(password);
}

void
ifm3d::O3R::Unlock(const std::string& password)
{
  return this->pImpl->Unlock(password);
}

ifm3d::PortInfo
ifm3d::O3R::Port(const std::string& port)
{
  return this->pImpl->Port(port);
}

std::vector<ifm3d::PortInfo>
ifm3d::O3R::Ports()
{
  return this->pImpl->Ports();
}

ifm3d::Device::device_family
ifm3d::O3R::WhoAmI()
{
  return device_family::O3R;
}

void
ifm3d::O3R::FromJSON(const json& j)
{
  this->Set(j);
  this->SaveInit();
}

json
ifm3d::O3R::ToJSON()
{
  return this->Get();
}

void
ifm3d::O3R::Reboot(const boot_mode& mode)
{
  this->pImpl->Reboot();
}

ifm3d::Device::swu_version
ifm3d::O3R::SwUpdateVersion()
{
  return ifm3d::Device::swu_version::SWU_V2;
}
