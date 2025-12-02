[CmdletBinding()]
param(
    $badParam,
    [Parameter(Mandatory=$False)][switch]$win64 = $false,
    [Parameter(Mandatory=$False)][string]$withVSPath = "",
    [Parameter(Mandatory=$False)][string]$withWinSDK = "",
    [Parameter(Mandatory=$False)][switch]$disableMetrics = $false
)
Set-StrictMode -Version Latest
# Powershell2-compatible way of forcing named-parameters
if ($badParam)
{
    if ($disableMetrics -and $badParam -eq "1")
    {
        Write-Warning "'disableMetrics 1' is deprecated, please change to 'disableMetrics' (without '1')."
    }
    else
    {
        throw "Only named parameters are allowed."
    }
}

if ($win64)
{
    Write-Warning "-win64 no longer has any effect; ignored."
}

if (-Not [string]::IsNullOrWhiteSpace($withVSPath))
{
    Write-Warning "-withVSPath no longer has any effect; ignored."
}

if (-Not [string]::IsNullOrWhiteSpace($withWinSDK))
{
    Write-Warning "-withWinSDK no longer has any effect; ignored."
}

$scriptsDir = split-path -parent $script:MyInvocation.MyCommand.Definition
$kmpkgRootDir = $scriptsDir
while (!($kmpkgRootDir -eq "") -and !(Test-Path "$kmpkgRootDir\.kmpkg-root"))
{
    Write-Verbose "Examining $kmpkgRootDir for .kmpkg-root"
    $kmpkgRootDir = Split-path $kmpkgRootDir -Parent
}

Write-Verbose "Examining $kmpkgRootDir for .kmpkg-root - Found"

# Read the kmpkg-tool config file to determine what release to download
$Config = ConvertFrom-StringData (Get-Content "$PSScriptRoot\kmpkg-tool-metadata.txt" -Raw)
$versionDate = $Config.KMPKG_TOOL_RELEASE_TAG

if ($env:PROCESSOR_ARCHITECTURE -eq 'ARM64' -or $env:PROCESSOR_IDENTIFIER -match "ARMv[8,9] \(64-bit\)") {
    & "$scriptsDir/tls12-download-arm64.exe" github.com "/microsoft/kmpkg-tool/releases/download/$versionDate/kmpkg-arm64.exe" "$kmpkgRootDir\kmpkg.exe"
} else {
    & "$scriptsDir/tls12-download.exe" github.com "/microsoft/kmpkg-tool/releases/download/$versionDate/kmpkg.exe" "$kmpkgRootDir\kmpkg.exe"
}

Write-Host ""

if ($LASTEXITCODE -ne 0)
{
    Write-Error "Downloading kmpkg.exe failed. Please check your internet connection, or consider downloading a recent kmpkg.exe from https://github.com/microsoft/kmpkg-tool with a browser."
    throw
}

& "$kmpkgRootDir\kmpkg.exe" version --disable-metrics

if ($disableMetrics)
{
    Set-Content -Value "" -Path "$kmpkgRootDir\kmpkg.disable-metrics" -Force
}
elseif (-Not (Test-Path "$kmpkgRootDir\kmpkg.disable-metrics"))
{
    # Note that we intentionally leave any existing kmpkg.disable-metrics; once a user has
    # opted out they should stay opted out.
    Write-Host @"
Telemetry
---------
kmpkg collects usage data in order to help us improve your experience.
The data collected by Microsoft is anonymous.
You can opt-out of telemetry by re-running the bootstrap-kmpkg script with -disableMetrics,
passing --disable-metrics to kmpkg on the command line,
or by setting the KMPKG_DISABLE_METRICS environment variable.

Read more about kmpkg telemetry at docs/about/privacy.md
"@
}
