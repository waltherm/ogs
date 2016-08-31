ADD_EXECUTABLE( Bettina 
    bettina.cpp
    Land.cpp
    Land.h
    Flora.cpp
    Flora.h
    Output.cpp
    Output.h
    Globals.cpp
    Globals.h
    Tree.cpp
    Tree.h
    Avicennia.cpp
    Avicennia.h
    Rhizophora.cpp
    Rhizophora.h
    )

ADD_VTK_DEPENDENCY(Bettina)

SET_TARGET_PROPERTIES( Bettina PROPERTIES FOLDER Utilities)
TARGET_LINK_LIBRARIES( Bettina
    MeshLib
    MeshGeoToolsLib
    GeoLib
    MathLib
    ${OGS_VTK_REQUIRED_LIBS}
)

