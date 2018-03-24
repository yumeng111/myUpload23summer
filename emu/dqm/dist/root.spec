%define	name emu-ROOT	
%define version 6.12.06	
%define release	1
%define root_dir /opt
%define os_name centos7
%define gcc_version gcc4_8_5

Name: %{name}
Version: %{version}
Release: %{release}.%{os_name}.%{gcc_version}
License: GNU Lesser General Public License
Group: Applications/Physics
BuildArch: x86_64
BuildRoot: %{_builddir}/%{name}-root
URL: http://root.cern.ch
Source: ftp://root.cern.ch/root/root_v%{version}.source.tar.gz
Vendor: CERN ROOT Team
Summary: Numerical data analysis framework (OO)
Packager: Victor Barashko <barvic@ufl.edu>
#Provides: ROOT-%{version} libCint.so libCore.so  libGpad.so  libGraf.so  libGraf3d.so  libGui.so  libHist.so  libMathCore.so  libMatrix.so  libNet.so  libPhysics.so  libPostscript.so  libProof.so  libProofPlayer.so  libRIO.so  libRint.so  libThread.so  libTree.so 
# Requires: 
#AutoReqProv: 0
AutoReq: 0

%global _use_internal_dependency_generator     0
%description
Package contains CERN ROOT Analysis Framework


%prep
exit 0

%build
exit 0

%install
ln -s %{root_dir} %{_buildrootdir}/%{name}-%{version}-%{release}.%{buildarch}/%{root_dir}
exit 0

%clean
exit 0

%files
%defattr(-,root,root)
/opt/cern/root_v%{version}

