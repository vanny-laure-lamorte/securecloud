param(
    [string]$Command
)

$RootDir = Split-Path $PSScriptRoot -Parent
Set-Location $RootDir

switch ($Command) {

    "cmake:configure" {
        Write-Host "[ Build the client target in Release mode... ]"
        cmake --build build --config Release -t client
    }

    "cmake:clear" {
        Write-Host "[ Clear CMake build files... ]"
        Remove-Item -Recurse -Force build
    }

    default {
        Write-Host "Unknown CMake command: $Command"
        exit 1
    }
}