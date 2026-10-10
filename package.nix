{
  lib,
  stdenv,
  cmake,
  ninja,
}:

stdenv.mkDerivation {
  pname = "cpp-core-utils";
  version = "2.0.0";

  nativeBuildInputs = [
    cmake
    ninja
  ];

  src = lib.cleanSourceWith {
    src = ./.;
    filter = path: type:
      let base = baseNameOf (toString path);
      in !(base == "build" || base == ".cache");
  };

  buildInputs = [ ];

  cmakeFlags = [
    "-DCORE_UTILS_BUILD_EXAMPLES=OFF"
  ];

  meta = with lib; {
    description = "Set of Core Utils for c++ projects";
    homepage = "https://github.com/bigmat18/cpp-core-utils";
    license = licenses.mit;
    maintainers = [ "bigmat18" ];
    platforms = platforms.unix;
  };
}
