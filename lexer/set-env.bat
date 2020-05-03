if "%1x" == "x" (
    set src_path=%~dp0src
    set gen_path=%~dp0gen
    set build_path=%~dp0build
) else  (
    set src_path=%1..\src
    set gen_path=%1..\gen
    set build_path=%1..\build
)