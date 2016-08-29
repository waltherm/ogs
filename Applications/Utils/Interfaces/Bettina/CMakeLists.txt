ADD_EXECUTABLE( Bettina 
    bettina.cpp
    globals.cpp
    globals.h
    land.cpp
    land.h
    tree.cpp
    tree.h
    avicennia.cpp
    avicennia.h
    rhizophora.cpp
    rhizophora.h
    )
SET_TARGET_PROPERTIES( Bettina PROPERTIES FOLDER Utilities)
TARGET_LINK_LIBRARIES( Bettina
    MeshLib
    GeoLib
    MathLib
    ${OGS_VTK_REQUIRED_LIBS}
)

