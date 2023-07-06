Name: altcenter
Version: 1.0
Release: alt1

Summary: Revised alterator
License: GPLv2+
Group: Other
URL: https://github.com/uyraq2001/alterator-module-browser

BuildPreReq: cmake rpm-macros-cmake
BuildRequires: cmake
BuildRequires: rpm-macros-cmake
BuildRequires: cmake-modules
BuildRequires: gcc-c++
BuildRequires: qt5-base-devel
BuildRequires: qt5-declarative-devel
BuildRequires: qt5-tools-devel
BuildRequires: qt5-base-common
BuildRequires: boost-devel-headers
BuildRequires: doxygen

Source0: %name-%version.tar

#Packager: kozyrevid <kozyrevid@basealt.ru>

%description
Alterator operating via D-Bus.

%prep
%setup -q

%build
%cmake
%cmake_build

%install
%cmakeinstall_std

%doc README.md
%doc INSTALL.md

%_bindir/alt-diagnostic-tool

%changelog
* Wed Jul 5 2023 Kozyrev Yuri <kozyrevid@altlinux.org> 1.0-alt1
- initial build
