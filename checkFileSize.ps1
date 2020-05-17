

class Files
{
    [string] $BasePath
    [System.Collections.ArrayList] $public;
    [System.Collections.ArrayList] $private;
    
    Files()
    {
        $this.BasePath    = ".\Source\$($(dir .\Source\ | Where-Object { $_.Mode -eq 'd-----'}).name)"
        $this.public  = New-Object System.Collections.ArrayList;
        $this.private = New-Object System.Collections.ArrayList;
    }

    [string] Path([string] $input){
        return $this.BasePath + "\" + $input + "\" ;
    }
};

class Core
{
    [Files] $files;

    Core()
    {
        $this.files = [Files]::new();
        $(dir $this.files.path("public")).
            foreach({ 
            if($_){
                $this.files.public.add($_) 
            }
        });

        $(dir $this.files.path("private")).
            foreach({ 
            if($_){
                $this.files.private.add($_) 
            }
        });
    }

    [void] PrintPublic()
    {
        $this.files.public.foreach({
            Write-Host "Public: $($(Dir $($this.files.path("public"))$_).Length) $_ ";
        })
    }

    [void] PrintPrivate()
    {
        $this.files.private.foreach({
            Write-Host "Private: $($(Dir $($this.files.path("private"))$_).Length) $_ ";
        })
    }
};


$core = [Core]::new();
$core.PrintPrivate();
write-host ''
$core.PrintPublic();

