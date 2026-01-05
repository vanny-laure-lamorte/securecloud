param(
    [string]$Command
)

$ScriptDir = $PSScriptRoot

function Show-Help {
    Write-Host ""
    Write-Host "SecureCloud CLI"
    Write-Host "Available commands"
    Write-Host ""
    Write-Host ""
    Write-Host "Translation"
    Write-Host "  translate:scan       Scan translation keys"
    Write-Host "  translate:update     Compile translation files"
    Write-Host "  translate:migrate    Build client with translations"
    Write-Host ""
    Write-Host "Migrations"
    Write-Host "  doctrine:generate    Generate Doctrine migrations"
    Write-Host "  doctrine:migrate     Run Doctrine migrations"
    Write-Host ""

}

# No argument → show help
if (-not $Command) {
    Show-Help
    exit 0
}

switch -Regex ($Command) {

    "^up:" {
        & "$ScriptDir\app.ps1" $Command
        break
    }

    "^translate:" {
        & "$ScriptDir\translate.ps1" $Command
        break
    }

    "^doctrine:" {
        & "$ScriptDir\doctrine.ps1" $Command
        break
    }

    default {
        Write-Host "Unknown command: $Command"
        Show-Help
        exit 1
    }
}
