
Remove-Item .\.vs\$($(dir .vs).Name)\v16\Browse.VC.db  -Force

Remove-Item .\.vs\$($(dir .vs).Name)\v16\Solution.VC.db  -Force

Remove-Item .\.vs\$($(dir .vs).Name)\v16\ipch\AutoPCH -Recurse -Force
