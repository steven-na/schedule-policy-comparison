{
    pkgs ? import <nixpkgs> { },
}:

pkgs.mkShell {
    name = "cpp-dev";

    buildInputs = with pkgs; [
        gcc
        gnumake
        gdb
        pkg-config
    ];

    shellHook = ''
        echo "C dev shell loaded"
    '';
}
