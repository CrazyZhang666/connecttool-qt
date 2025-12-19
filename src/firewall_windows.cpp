#ifdef _WIN32

#include "firewall_windows.h"

#include <cstdlib>
#include <sstream>
#include <string>

namespace {
std::string escape_ps(const std::string &value) {
  std::string escaped;
  escaped.reserve(value.size());
  for (char c : value) {
    if (c == '\'') {
      escaped += "''";
    } else {
      escaped.push_back(c);
    }
  }
  return escaped;
}
} // namespace

bool ensureTcpFirewallRule(const char *ruleName, int port) {
  if (port <= 0 || ruleName == nullptr || ruleName[0] == '\0') {
    return false;
  }
  const std::string escapedName = escape_ps(ruleName);
  std::ostringstream ps;
  ps << "powershell -Command \"$ErrorActionPreference='SilentlyContinue'; "
     << "Remove-NetFirewallRule -DisplayName '" << escapedName
     << "' -ErrorAction SilentlyContinue; "
     << "New-NetFirewallRule -DisplayName '" << escapedName
     << "' -Direction Inbound -Action Allow -Protocol TCP -LocalPort "
     << port << " -Enabled True\"";
  return ::system(ps.str().c_str()) == 0;
}

bool ensureTunFirewallRule(const char *ruleName, const char *interfaceAlias) {
  if (ruleName == nullptr || ruleName[0] == '\0' || interfaceAlias == nullptr ||
      interfaceAlias[0] == '\0') {
    return false;
  }
  const std::string escapedName = escape_ps(ruleName);
  const std::string escapedAlias = escape_ps(interfaceAlias);
  std::ostringstream ps;
  ps << "powershell -Command \"$ErrorActionPreference='SilentlyContinue'; "
     << "Remove-NetFirewallRule -DisplayName '" << escapedName
     << "' -ErrorAction SilentlyContinue; "
     << "New-NetFirewallRule -DisplayName '" << escapedName
     << "' -Direction Inbound -Action Allow -Protocol Any "
     << "-InterfaceAlias '" << escapedAlias << "' -Enabled True\"";
  return ::system(ps.str().c_str()) == 0;
}

#endif
