#pragma once

#ifdef _WIN32
bool ensureTcpFirewallRule(const char *ruleName, int port);
bool ensureTunFirewallRule(const char *ruleName, const char *interfaceAlias);
#else
inline bool ensureTcpFirewallRule(const char *, int) { return true; }
inline bool ensureTunFirewallRule(const char *, const char *) { return true; }
#endif
