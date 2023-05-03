
$KNetPath = "F:\Development2\kNet"
Push-Location "$KNetPath"

$CMakeFiles = gci .\ -recurse | where {$_.extension -in ".vcxproj",".sln",".filters",".cmake"}

Remove-Item $CMakeFiles -Force -ErrorAction Ignore -Verbose