{
  lib,
  stdenv,
  cmake,
  ninja,
}:

stdenv.mkDerivation {
  pname = "cpp-core-utils";
  version = "1.0.0";

  nativeBuildInputs = [
    cmake
    ninja
  ];

  src = ./.;

  buildInputs = [ ];

  cmakeFlags = [
    "-DCPPUTILS_BUILD_EXAMPLES=OFF"
  ];

  meta = with lib; {
    description = "Set of Core Utils for c++ projects";
    homepage = "https://github.com/bigmat18/cpp-core-utils";
    license = licenses.mit;
    maintainers = [ "bigmat18" ];
    platforms = platforms.unix;
  };
}
