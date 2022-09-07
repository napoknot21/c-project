# CMakeList.txt�: fichier projet CMake de niveau sup�rieur, effectuez une configuration globale
# et incluez les sous-projets ici.
#
cmake_minimum_required(VERSION 3.8)

project(c-project VERSION 0.0.1 LANGUAGES C)

# Incluez les sous-projets.
add_subdirectory(Library)
add_subdirectory(Objects)
add_subdirectory(Calc)

enable_testing()
add_subdirectory(Test)
