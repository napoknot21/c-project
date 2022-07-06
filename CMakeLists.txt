# CMakeList.txt : fichier projet CMake de niveau supérieur, effectuez une configuration globale
# et incluez les sous-projets ici.
#
cmake_minimum_required (VERSION 3.8)

project (c-project VERSION 0.0.1 LANGUAGES C)

# Incluez les sous-projets.
add_subdirectory (Calc)
add_subdirectory (Library)
add_subdirectory (Objects)		
