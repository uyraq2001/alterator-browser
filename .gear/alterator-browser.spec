%define _unpackaged_files_terminate_build 1

Name: alterator-browser
Version: 0.1.0
Release: alt1

Summary: Revised alterator
License: GPLv2+
Group: Other
URL: https://github.com/uyraq2001/alterator-module-browser

BuildRequires(pre): rpm-macros-cmake
BuildRequires: cmake
BuildRequires: gcc-c++
BuildRequires: qt5-base-devel
BuildRequires: qt5-tools-devel
BuildRequires: qt5-base-common
BuildRequires: boost-devel-headers

Requires: alterator
Requires: alterator-manager
Requires: alterator-module-executor
Requires: alterator-module-legacy
Requires: alterator-module-categories
Requires: alterator-interface-application

Source0: %name-%version.tar

%description
Alterator operating via D-Bus.

%prep
%setup

%build
%cmake
%cmake_build

%install
%cmakeinstall_std

%files
%_bindir/alterator-browser
%doc README.md INSTALL.md

%changelog
* Mon Jan 15 2024 Aleksey Saprunov <sav@altlinux.org> 0.1.0-alt1
- added new objects support
* Wed Jul 5 2023 Kozyrev Yuri <kozyrevid@altlinux.org> 0.0.1-alt1
- initial build
