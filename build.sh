#!/bin/bash

# Cedis Build Script
# Professional-grade build system for cross-platform compilation
# Supports CI/CD environments and local development

set -euo pipefail  # Strict error handling

# Configuration
readonly SCRIPT_NAME="build.sh"
readonly SCRIPT_VERSION="1.0.0"
readonly PROJECT_NAME="Cedis"
readonly BUILD_DIR="build"
readonly DEFAULT_BUILD_TYPE="Release"

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

# Check if command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Detect number of CPU cores for parallel build
get_cpu_cores() {
    if command_exists nproc; then
        nproc
    elif command_exists sysctl; then
        sysctl -n hw.ncpu
    else
        echo "4"  # Default fallback
    fi
}

# Validate project structure
validate_project() {
    log_info "Validating project structure..."
    
    local required_files=(
        "CMakeLists.txt"
        "src/main.cpp"
        "include/connection.h"
        "include/server.h"
        "include/database.h"
    )
    
    for file in "${required_files[@]}"; do
        if [[ ! -f "$file" ]]; then
            log_error "Required file not found: $file"
            log_error "Please run this script from the Cedis project root directory."
            return 1
        fi
    done
    
    log_success "Project structure validated"
}

# Clean build directory
clean_build() {
    if [[ -d "$BUILD_DIR" ]]; then
        log_info "Cleaning build directory..."
        rm -rf "$BUILD_DIR"
        log_success "Build directory cleaned"
    fi
}

# Create build directory
create_build_dir() {
    log_info "Creating build directory..."
    mkdir -p "$BUILD_DIR"
    cd "$BUILD_DIR"
    log_success "Build directory created: $(pwd)"
}

# Configure with CMake
configure_cmake() {
    local build_type="${1:-$DEFAULT_BUILD_TYPE}"
    local cmake_args=(
        "-DCMAKE_BUILD_TYPE=$build_type"
        "-DCMAKE_CXX_STANDARD=17"
        "-DCMAKE_CXX_STANDARD_REQUIRED=ON"
    )
    
    # Add CI-specific optimizations
    if is_ci; then
        cmake_args+=(
            "-DCMAKE_BUILD_PARALLEL_LEVEL=$(get_cpu_cores)"
        )
    fi
    
    log_info "Configuring with CMake..."
    log_info "Build type: $build_type"
    log_info "CMake arguments: ${cmake_args[*]}"
    
    # Detect CMake command
    local cmake_cmd
    if command_exists cmake3; then
        cmake_cmd="cmake3"
    elif command_exists cmake; then
        cmake_cmd="cmake"
    else
        log_error "CMake not found"
        return 1
    fi
    
    # Run CMake configuration
    if ! $cmake_cmd .. "${cmake_args[@]}"; then
        log_error "CMake configuration failed"
        return 1
    fi
    
    log_success "CMake configuration completed"
}

# Build the project
build_project() {
    local build_type="${1:-$DEFAULT_BUILD_TYPE}"
    local cores
    cores=$(get_cpu_cores)
    
    log_info "Building $PROJECT_NAME..."
    log_info "Using $cores CPU cores for parallel build"
    log_info "Build type: $build_type"
    
    # Detect build system
    local build_cmd=""
    local build_args=()
    
    if command_exists make; then
        build_cmd="make"
        build_args=("-j$cores")
    elif command_exists ninja; then
        build_cmd="ninja"
    else
        log_error "No build system found (make or ninja)"
        return 1
    fi
    
    # Run build
    if ! $build_cmd "${build_args[@]}"; then
        log_error "Build failed"
        return 1
    fi
    
    log_success "Build completed successfully"
}

# Run tests (if available)
run_tests() {
    if [[ -f "CTestTestfile.cmake" ]]; then
        log_info "Running tests..."
        if command_exists ctest; then
            if ctest --output-on-failure; then
                log_success "All tests passed"
            else
                log_warning "Some tests failed"
                return 1
            fi
        fi
    else
        log_info "No tests found, skipping test execution"
    fi
}

# Verify build artifacts
verify_build() {
    log_info "Verifying build artifacts..."
    
    local executable_name="$PROJECT_NAME"
    if [[ "$(uname -s)" == "MINGW"* ]] || [[ "$(uname -s)" == "MSYS"* ]]; then
        executable_name="${PROJECT_NAME}.exe"
    fi
    
    if [[ -f "$executable_name" ]]; then
        log_success "Executable found: $executable_name"
        
        # Show file size
        local size
        size=$(du -h "$executable_name" | cut -f1)
        log_info "Executable size: $size"
        
        # Show file type
        if command_exists file; then
            local file_type
            file_type=$(file "$executable_name")
            log_info "File type: $file_type"
        fi
    else
        log_error "Executable not found: $executable_name"
        return 1
    fi
}

# Print build summary
print_summary() {
    local build_type="${1:-$DEFAULT_BUILD_TYPE}"
    
    log_success ""
    log_success "🎉 Build completed successfully!"
    log_success "================================="
    log_info "Project: $PROJECT_NAME"
    log_info "Build type: $build_type"
    log_info "Build directory: $(pwd)"
    log_info "Platform: $(uname -s)"
    log_info "Architecture: $(uname -m)"
    log_success ""
    log_success "🚀 You can now run $PROJECT_NAME:"
    log_success "   ./build/$PROJECT_NAME"
    log_success ""
    log_success "🔧 Or test with redis-cli:"
    log_success "   redis-cli -h 127.0.0.1 -p 6969"
    log_success ""
}

# Main build function
main_build() {
    local build_type="${1:-$DEFAULT_BUILD_TYPE}"
    local clean="${2:-false}"
    
    log_info "🔨 $PROJECT_NAME Build Script v$SCRIPT_VERSION"
    log_info "============================================="
    
    # Check if running in CI
    if is_ci; then
        log_info "Running in CI/CD environment"
    fi
    
    # Validate project structure
    validate_project
    
    # Clean if requested
    if [[ "$clean" == "true" ]]; then
        clean_build
    fi
    
    # Create and enter build directory
    create_build_dir
    
    # Configure with CMake
    configure_cmake "$build_type"
    
    # Build the project
    build_project "$build_type"
    
    # Run tests
    run_tests
    
    # Verify build artifacts
    verify_build
    
    # Print summary
    print_summary "$build_type"
}

# Handle script arguments
case "${1:-}" in
    --help|-h)
        echo "Usage: $SCRIPT_NAME [OPTIONS] [BUILD_TYPE]"
        echo ""
        echo "Options:"
        echo "  --help, -h        Show this help message"
        echo "  --version, -v     Show version information"
        echo "  --clean, -c       Clean build directory before building"
        echo "  --debug           Build in Debug mode"
        echo "  --release         Build in Release mode (default)"
        echo "  --test            Run tests after building"
        echo ""
        echo "Build types:"
        echo "  Debug             Debug build with symbols"
        echo "  Release           Release build (default)"
        echo "  RelWithDebInfo    Release build with debug info"
        echo "  MinSizeRel        Minimal size release build"
        echo ""
        echo "Examples:"
        echo "  $SCRIPT_NAME                    # Build in Release mode"
        echo "  $SCRIPT_NAME --debug            # Build in Debug mode"
        echo "  $SCRIPT_NAME --clean            # Clean and build"
        echo "  $SCRIPT_NAME Debug --clean      # Clean and build in Debug mode"
        exit 0
        ;;
    --version|-v)
        echo "$SCRIPT_NAME v$SCRIPT_VERSION"
        exit 0
        ;;
    --clean|-c)
        main_build "${2:-$DEFAULT_BUILD_TYPE}" "true"
        ;;
    --debug)
        main_build "Debug"
        ;;
    --release)
        main_build "Release"
        ;;
    --test)
        main_build "${2:-$DEFAULT_BUILD_TYPE}"
        run_tests
        ;;
    Debug|Release|RelWithDebInfo|MinSizeRel)
        main_build "$1"
        ;;
    "")
        main_build "$DEFAULT_BUILD_TYPE"
        ;;
    *)
        log_error "Unknown option: $1"
        log_info "Use --help for usage information"
        exit 1
        ;;
esac 