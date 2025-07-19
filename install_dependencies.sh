#!/bin/bash

# Cedis Dependency Installer
# Professional-grade dependency management for cross-platform builds
# Supports CI/CD environments and local development

set -euo pipefail  # Strict error handling

# Configuration
readonly SCRIPT_NAME="install_dependencies.sh"
readonly SCRIPT_VERSION="1.0.0"
readonly REQUIRED_BOOST_VERSION="1.70"
readonly REQUIRED_CMAKE_VERSION="3.20"

# Colors for output (only if terminal supports it)
if [[ -t 1 ]] && [[ -n "${TERM:-}" ]]; then
    readonly RED='\033[0;31m'
    readonly GREEN='\033[0;32m'
    readonly YELLOW='\033[1;33m'
    readonly BLUE='\033[0;34m'
    readonly NC='\033[0m' # No Color
else
    readonly RED=''
    readonly GREEN=''
    readonly YELLOW=''
    readonly BLUE=''
    readonly NC=''
fi

# Logging functions
log_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

log_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

log_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

log_error() {
    echo -e "${RED}[ERROR]${NC} $1" >&2
}

# Check if running in CI/CD environment
is_ci() {
    [[ -n "${CI:-}" ]] || [[ -n "${GITHUB_ACTIONS:-}" ]] || [[ -n "${TRAVIS:-}" ]] || [[ -n "${CIRCLECI:-}" ]]
}

# Check if running with sudo privileges
is_sudo() {
    [[ $EUID -eq 0 ]]
}

# Version comparison function
version_compare() {
    if [[ $1 == $2 ]]; then
        return 0
    fi
    local IFS=.
    local i ver1=($1) ver2=($2)
    for ((i=${#ver1[@]}; i<${#ver2[@]}; i++)); do
        ver1[i]=0
    done
    for ((i=0; i<${#ver1[@]}; i++)); do
        if [[ -z ${ver2[i]} ]]; then
            ver2[i]=0
        fi
        if ((10#${ver1[i]} > 10#${ver2[i]})); then
            return 1
        fi
        if ((10#${ver1[i]} < 10#${ver2[i]})); then
            return 2
        fi
    done
    return 0
}

# Check if command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Detect operating system
detect_os() {
    local os
    case "$(uname -s)" in
        Linux*)     os="linux";;
        Darwin*)    os="macos";;
        CYGWIN*|MINGW*|MSYS*) os="windows";;
        *)          os="unknown";;
    esac
    echo "$os"
}

# Install Boost on Linux
install_boost_linux() {
    log_info "Installing Boost on Linux..."
    
    local package_manager=""
    local install_cmd=""
    
    # Detect package manager
    if command_exists apt-get; then
        package_manager="apt"
        install_cmd="sudo apt-get update && sudo apt-get install -y"
    elif command_exists yum; then
        package_manager="yum"
        install_cmd="sudo yum install -y"
    elif command_exists dnf; then
        package_manager="dnf"
        install_cmd="sudo dnf install -y"
    elif command_exists pacman; then
        package_manager="pacman"
        install_cmd="sudo pacman -S --noconfirm"
    elif command_exists zypper; then
        package_manager="zypper"
        install_cmd="sudo zypper install -y"
    else
        log_error "No supported package manager found"
        return 1
    fi
    
    log_info "Using package manager: $package_manager"
    
    # Install dependencies based on package manager
    case $package_manager in
        apt)
            eval "$install_cmd libboost-system-dev libboost-thread-dev cmake build-essential"
            ;;
        yum|dnf)
            eval "$install_cmd boost-devel cmake gcc-c++"
            ;;
        pacman)
            eval "$install_cmd boost cmake base-devel"
            ;;
        zypper)
            eval "$install_cmd boost-devel cmake gcc-c++"
            ;;
    esac
    
    log_success "Boost installed successfully on Linux"
}

# Install Boost on macOS
install_boost_macos() {
    log_info "Installing Boost on macOS..."
    
    if command_exists brew; then
        log_info "Using Homebrew"
        brew install boost cmake
    elif command_exists port; then
        log_info "Using MacPorts"
        sudo port install boost cmake
    else
        log_error "No package manager found"
        log_info "Please install Homebrew:"
        log_info "  /bin/bash -c \"\$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)\""
        return 1
    fi
    
    log_success "Boost installed successfully on macOS"
}

# Install Boost on Windows
install_boost_windows() {
    log_info "Installing Boost on Windows..."
    
    if command_exists vcpkg; then
        log_info "Using vcpkg"
        vcpkg install boost-system boost-thread
    else
        log_error "vcpkg not found"
        log_info "Please install vcpkg: https://github.com/microsoft/vcpkg"
        return 1
    fi
    
    log_success "Boost installed successfully on Windows"
}

# Verify installation
verify_installation() {
    log_info "Verifying installation..."
    
    # Check CMake
    if ! command_exists cmake; then
        log_error "CMake not found"
        return 1
    fi
    
    local cmake_version
    cmake_version=$(cmake --version | head -n1 | cut -d' ' -f3)
    log_info "CMake version: $cmake_version"
    
    # Check Boost (if possible)
    if command_exists pkg-config; then
        if pkg-config --exists boost; then
            local boost_version
            boost_version=$(pkg-config --modversion boost 2>/dev/null || echo "unknown")
            log_info "Boost version: $boost_version"
        fi
    fi
    
    log_success "Installation verification completed"
}

# Main function
main() {
    log_info "🚀 Cedis Dependency Installer v$SCRIPT_VERSION"
    log_info "=============================================="
    
    # Check if running in CI
    if is_ci; then
        log_info "Running in CI/CD environment"
    fi
    
    # Detect OS
    local os
    os=$(detect_os)
    log_info "Detected OS: $os"
    
    if [[ "$os" == "unknown" ]]; then
        log_error "Unsupported operating system"
        exit 1
    fi
    
    # Install based on OS
    case $os in
        linux)
            install_boost_linux
            ;;
        macos)
            install_boost_macos
            ;;
        windows)
            install_boost_windows
            ;;
        *)
            log_error "Unsupported OS: $os"
            exit 1
            ;;
    esac
    
    # Verify installation
    verify_installation
    
    log_success "✅ Dependencies installed successfully!"
    log_info ""
    log_info "🔨 Next steps:"
    log_info "   ./build.sh"
    log_info ""
    log_info "🚀 Or manually:"
    log_info "   mkdir build && cd build"
    log_info "   cmake .."
    log_info "   make"
}

# Handle script arguments
case "${1:-}" in
    --help|-h)
        echo "Usage: $SCRIPT_NAME [OPTIONS]"
        echo "Options:"
        echo "  --help, -h     Show this help message"
        echo "  --version, -v  Show version information"
        echo "  --verify       Only verify existing installation"
        exit 0
        ;;
    --version|-v)
        echo "$SCRIPT_NAME v$SCRIPT_VERSION"
        exit 0
        ;;
    --verify)
        verify_installation
        exit 0
        ;;
    "")
        main
        ;;
    *)
        log_error "Unknown option: $1"
        log_info "Use --help for usage information"
        exit 1
        ;;
esac 