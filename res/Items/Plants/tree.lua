

 ICON_TEXTURE_FILE = "res/Items/Icons/seed.png"
-- IMG_ICON_POS_X = 173
--IMG_ICON_POS_Y = 92
 IMG_ICON_WIDTH = 512
 IMG_ICON_HEIGHT = 512
 
 MODEL_WIDTH = 1
 MODEL_LENGTH = 1
 MODEL_HEIGHT = -1

 TYPE = "Plant"
 NAME = "Tree"

 HARVESTABLE = 1

 STAGE = {}
 
 STAGE[1] = {}
 STAGE[1]["file"] = "res/Items/Plants/DirtMound.gpb"
 STAGE[1]["texture"] = "res/Mats/Dirt.png"
 STAGE[1]["normal"] = "res/Items/Plants/mound-normal.png"
 STAGE[1]["id"] = "Mound"
 STAGE[1]["jointCount"] = 0
 STAGE[1]["radius"] = -1
 STAGE[1]["height"] = 0.1
 STAGE[1]["duration"] = 2
 
 STAGE[2] = {}
 STAGE[2]["file"] = "res/Items/Plants/tree.gpb"
 STAGE[2]["texture"] = "res/Items/Plants/Tree.png"
 STAGE[2]["normal"] = "res/Items/Plants/Tree-Normal.png"
 STAGE[2]["id"] = "Sap"
 STAGE[2]["jointCount"] = 0
 STAGE[2]["radius"] = -1
 STAGE[2]["height"] = 0.2
 STAGE[2]["duration"] = 2
 
 STAGE[3] = {}
 STAGE[3]["file"] = "res/Items/Plants/tree.gpb"
 STAGE[3]["texture"] = "res/Items/Plants/Tree.png"
 STAGE[3]["normal"] = "res/Items/Plants/Tree-Normal.png"
 STAGE[3]["id"] = "Growing"
 STAGE[3]["radius"] = 0.3
 STAGE[3]["height"] = 2
 STAGE[3]["jointCount"] = 30
 STAGE[3]["animation"] = "res/Items/Plants/Tree-growing.animation"
 STAGE[3]["duration"] = 2
 
 STAGE[4] = {}
 STAGE[4]["file"] = "res/Items/Plants/tree.gpb"
 STAGE[4]["texture"] = "res/Items/Plants/Tree.png"
 STAGE[4]["normal"] = "res/Items/Plants/Tree-Normal.png"
 STAGE[4]["id"] = "Tree"
 STAGE[4]["radius"] = 0.5
 STAGE[4]["height"] = 3
 STAGE[4]["jointCount"] = 20
 STAGE[4]["animation"] = "res/Items/Plants/Tree-Tree.animation"
 STAGE[4]["duration"] = -1
 
  -- ATTACH_POINTS = {
  -- [1] = { -0.51399, 1.55027, 2.99985},
  -- [2] = { 1.07644, -1.02015, 2.746},
  -- [3]={ -0.37213, -2.06889, 2.90331},
  -- [4]={ -1.43398, -1.38721, 3.2507},
 -- [5]={ -1.36187, 0.35631, 3.08028}
 -- }
 
   ATTACH_POINTS = {
  [1] = { -0.513, 1.550, 2.999},
  [2] = { 1.076, -1.020, 2.746},
  [3]={ -0.372, -2.068, 2.903},
  [4]={ -1.43398, -1.38721, 3.250},
 [5]={ -1.361, 0.356, 3.080}
 }

-- POO = {}
-- --ATTACH_POINTS[1] = { -0.51399, 1.55027, 2.99985}
-- POO[1] = {}
-- POO[1][1] = -0.51
-- POO[1][2] = 1.55
-- POO[1][3] = 2.99
--ATTACH_POINTS[2] = { 1.07644, -1.02015, 2.746}

TEST = {}
TEST[1] = {}
TEST[1][1] = -3.5
TEST[1][2] = 3.6
TEST[1][3] = -3.7
TEST[1][4] = 3.8

 
CROP_FILE = "res/Items/Crops/apple.lua"

 
 
