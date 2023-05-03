

$Content = Get-Content "$PSScriptRoot\kNet.vcxproj"
$OutputDirectoryOpen = '<OutputDirectory'
$OutputDirectoryClose = '</OutputDirectory'
$ClCompileOpen = '<ClInclude Include='
$ClCompileClose = '</ClCompile'
$SectionClose = '/>'

ForEach($line in $Content){

    if([string]::IsNullOrEmpty($line)) { break }
    if( ($line.Contains($OutputDirectoryOpen)) -And ($line.Contains($OutputDirectoryClose))){ break }
    Add-Content -Path "$PSScriptRoot\kNet-new.vcxproj" -Value "$line"
}

<#
    if( ($line.Contains($ClCompileOpen)) ){
        #if(($line.Contains($ClCompileClose)) -Or ($line.Contains($SectionClose))) { break ; }
        $newline = $line.Replace('F:\Development2\kNet\include\','$(ProjectDir)..\include\')    
        #$newline = $newline.Replace('>' , ' />')
        $newline
    }
    
}
#>