
gci env: | where Name -match Boos | % {
    $AddressPretty = "{0}:\{1}" -f $_.PSDrive, $_.Name
    $Address = "{0}" -f $_.PSPath
    Write-Host "DELETE ENVIRONMENT VALUE: " -n -f DarkYellow
    Write-Host $AddressPretty -f DarkRed
    remove-item $Address -Force -ErrorAction Stop
}

Set-EnvironmentVariable -Name "BOOST_ROOT" -Value "c:/boost-1_82" -Scope User
Set-EnvironmentVariable -Name "BOOST_ROOT" -Value "c:/boost-1_82" -Scope Session