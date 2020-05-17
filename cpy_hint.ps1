cp .\cpp.hint .\Source\TheGame
cp .\cpp.hint .\Source\TheGame\Public
cp .\cpp.hint .\Source\TheGame\Private

cp .\cpp.hint .\Intermediate\ProjectFiles
cp .\cpp.hint .\Source
cp .\cpp.hint '.\Intermediate\ProjectFiles'

cp .\cpp.hint .\Intermediate\Build\Win64\UE4Editor\Inc\TheGame

# $($(dir .\Source\ | Where-Object { $_.Mode -eq 'd-----'}).name)


# D:\AID\UE4\UE_4.25\Engine\Source\Runtime\Core\Public\Containers\Set.h:776]