
class Files
{
    [System.Collections.ArrayList] $files;

    Files()
    {
        $this.files = New-Object System.Collections.ArrayList;

        $this.files.add(".vs");
        $this.files.add("Binaries");
        $this.files.add("Build");
        $this.files.add("Intermediate");
        $this.files.add("Saved");

        $sln_file = $(dir | Where-Object { $_.Name -like "*.sln" }).Name
        if($sln_file){
            $this.files.add($sln_file);
        }
    }

    [void] Wipe()
    {
        $this.files.foreach({
            if(Test-Path($_))
            {
                Write-Host "Deleting: " $_
                Remove-Item $_ -Force -Recurse
            }
        });
    }
};


$files = [Files]::new();
$files.Wipe();
