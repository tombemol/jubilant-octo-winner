New-Item -ItemType Directory -Force -Path build | Out-Null
gcc -std=c11 -Wall -Wextra -Wpedantic -O2 -Iinclude src/*.c -o build/livraria.exe
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
Write-Host "Compilado em build/livraria.exe"
