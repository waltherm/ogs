ADD_EXECUTABLE( Bettina 
    bettina.cpp
    Flora.cpp
    Flora.h
    Globals.cpp
    Globals.h
    Land.cpp
    Land.h
    Tree.cpp
    Tree.h
    Avicennia.cpp
    Avicennia.h
    Rhizophora.cpp
    Rhizophora.h
    )
SET_TARGET_PROPERTIES( Bettina PROPERTIES FOLDER Utilities)
TARGET_LINK_LIBRARIES( Bettina
    MeshLib
    MeshGeoToolsLib
    GeoLib
    MathLib
    ${OGS_VTK_REQUIRED_LIBS}
)

