
gci env: | where Name -match Boos | % {
    $AddressPretty = "{0}:\{1}" -f $_.PSDrive, $_.Name
    $Address = "{0}" -f $_.Value
    Write-Host "DELETE ENVIRONMENT VALUE: " -n -f DarkYellow
    Write-Host "$AddressPretty $Address" -f DarkRed
}
