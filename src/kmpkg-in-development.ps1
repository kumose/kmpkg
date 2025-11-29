# Copyright (c) Microsoft Corporation.
# Licensed under the MIT License.

$ENV:NODE_OPTIONS="--enable-source-maps"

# setup the postscript file
# Generate 31 bits of randomness, to avoid clashing with concurrent executions.
$env:Z_KMPKG_POSTSCRIPT = Join-Path ([System.IO.Path]::GetTempPath()) "KMPKG_tmp_$(Get-Random -SetSeed $PID).ps1"

[string]$KMPKG = "$PSScriptRoot/kmpkg"
# The variable:IsWindows test is a workaround for $IsWindows not existing in Windows PowerShell
if (-Not (Test-Path variable:IsWindows) -Or $IsWindows) {
  $KMPKG += ".exe"
}

& $KMPKG @args

# dot-source the postscript file to modify the environment
if (Test-Path $env:Z_KMPKG_POSTSCRIPT) {
  $postscr = Get-Content -Raw $env:Z_KMPKG_POSTSCRIPT
  if( $postscr ) {
    iex $postscr
  }

  Remove-Item -Force -ea 0 $env:Z_KMPKG_POSTSCRIPT
}

Remove-Item env:Z_KMPKG_POSTSCRIPT
