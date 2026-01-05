param(
    [string]$Command
)

$RootDir = Split-Path $PSScriptRoot -Parent

# Paths
$ClientSrc = Join-Path $RootDir "client\src\main.cpp"
$I18nPath  = Join-Path $RootDir "common\i18n"
$BuildDir = Join-Path $RootDir "build"

function Show-TranslateHelp {
    Write-Host ""
    Write-Host "Translation"
    Write-Host ""
    Write-Host "  translate:scan       Scan translation keys"
    Write-Host "  translate:update     Compile translation files (from .ts to .qm)"
    Write-Host "  translate:migrate    Build client with translations"
    Write-Host ""
}

if (-not $Command -or $Command -eq "translate") {
    Show-TranslateHelp
    exit 0
}

$env:PATH = "C:\Qt\6.7.1\mingw_64\bin;" + $env:PATH

switch ($Command) {

    # Scan
    "translate:scan" {
        Write-Host "[1/3] Scanning translation keys"

        if (-not (Test-Path $ClientSrc)) {
            Write-Host "Source file not found: $ClientSrc"
            exit 1
        }

        lupdate $ClientSrc `
            -ts "$I18nPath\fr_FR.ts" `
                "$I18nPath\en_US.ts" `
                "$I18nPath\es_ES.ts"

        Write-Host "Scan completed."
    }

    # Update
    "translate:update" {
        Write-Host "[2/3] Compiling translation files"

        lrelease `
            "$I18nPath\fr_FR.ts" `
            "$I18nPath\en_US.ts" `
            "$I18nPath\es_ES.ts"

        Write-Host " Translations compiled."
    }

    # Compile
    "translate:compile" {
        Write-Host "[3/3] Building client"

        cmake --build $BuildDir --target client

        Write-Host "Build completed."
    }

    default {
        Write-Host "Unknown translation command: $Command"
        Show-TranslateHelp
        exit 1
    }
}
