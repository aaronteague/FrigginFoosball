WORLD_BLOCK_LENGTH = 8
BLOCK_TILE_LENGTH = 16
NUM_OF_PONDS = 3
MIN_NUM_OF_TREES = 150
MAX_NUM_OF_TREES = 250
NUM_OF_NEIGHBORS_TO_START = 3
MAX_NUM_OF_NEIGHBORS = 10







--list of neighbors the game could use
NUM_OF_NEIGHBOR_FILES = 5
Neighbor = {}
Neighbor[1] = "res/luaFiles/Bobby.lua"
Neighbor[2] = "res/luaFiles/Kate.lua"
Neighbor[3] = "res/luaFiles/Kristen.lua"
Neighbor[4] = "res/luaFiles/Kristen.lua"
Neighbor[5] = "res/luaFiles/Kristen.lua"

--make a table Blocks
BlockFile = {}



NUM_OF_BOTTOM_BLOCK_FILES = 1
BlockFile["bottom"] = {}
BlockFile["bottom"][1] = "res/Blocks/bottom.lua"

NUM_OF_BOTTOM_CORNER_TOP_BOTH_BLOCK_FILES = 1
BlockFile["bottom-corner-top-both"] = {}
BlockFile["bottom-corner-top-both"][1] = "res/Blocks/bottom-corner-top-both.lua"

NUM_OF_EMPTY_BLOCK_FILES = 1
BlockFile["empty"] = {}
BlockFile["empty"][1] = "res/Blocks/empty.lua"

NUM_OF_LEFT_BLOCK_FILES = 1
BlockFile["left"] = {}
BlockFile["left"][1] = "res/Blocks/left.lua"

NUM_OF_LEFT_CORNER_RIGHT_BOTH_BLOCK_FILES = 1
BlockFile["left-corner-right-both"] = {}
BlockFile["left-corner-right-both"][1] = "res/Blocks/left-corner-right-both.lua"

NUM_OF_LEFT_RIGHT_BLOCK_FILES = 1
BlockFile["left-right"] = {}
BlockFile["left-right"][1] = "res/Blocks/left-right.lua"

NUM_OF_LEFT_RIGHT_BRIDGE_BLOCK_FILES = 1
BlockFile["left-right-bridge"] = {}
BlockFile["left-right-bridge"][1] = "res/Blocks/left-right-bridge.lua"

NUM_OF_POND_BLOCK_FILES = 1
BlockFile["pond"] = {}
BlockFile["pond"][1] = "res/Blocks/pond.lua"

NUM_OF_RIGHT_BLOCK_FILES = 1
BlockFile["right"] = {}
BlockFile["right"][1] = "res/Blocks/right.lua"

NUM_OF_RIGHT_CORNER_LEFT_BOTH_BLOCK_FILES = 1
BlockFile["right-corner-left-both"] = {}
BlockFile["right-corner-left-both"][1] = "res/Blocks/right-corner-left-both.lua"

NUM_OF_TOP_BLOCK_FILES = 1
BlockFile["top"] = {}
BlockFile["top"][1] = "res/Blocks/top.lua"

NUM_OF_TOP_BOTTOM_BLOCK_FILES = 1
BlockFile["top-bottom"] = {}
BlockFile["top-bottom"][1] = "res/Blocks/top-bottom.lua"

NUM_OF_TOP_BOTTOM_BRIDGE_BLOCK_FILES = 1
BlockFile["top-bottom-bridge"] = {}
BlockFile["top-bottom-bridge"][1] = "res/Blocks/top-bottom-bridge.lua"

NUM_OF_TOP_CORNER_BOTTOM_BOTH_BLOCK_FILES = 1
BlockFile["top-corner-bottom-both"] = {}
BlockFile["top-corner-bottom-both"][1] = "res/Blocks/top-corner-bottom-both.lua"

NUM_OF_TOP_LEFT_BLOCK_FILES = 1
BlockFile["top-left"] = {}
BlockFile["top-left"][1] = "res/Blocks/top-left.lua"

NUM_OF_TOP_LEFT_CORNER_BOTTOM_RIGHT_BLOCK_FILES = 1
BlockFile["top-left-corner-bottom-right"] = {}
BlockFile["top-left-corner-bottom-right"][1] = "res/Blocks/top-left-corner-bottom-right.lua"

NUM_OF_TOP_RIGHT_BLOCK_FILES = 1
BlockFile["top-right"] = {}
BlockFile["top-right"][1] = "res/Blocks/top-right.lua"

NUM_OF_TOP_RIGHT_CORNER_BOTTOM_LEFT_BLOCK_FILES = 1
BlockFile["top-right-corner-bottom-left"] = {}
BlockFile["top-right-corner-bottom-left"][1] = "res/Blocks/top-right-corner-bottom-left.lua"

NUM_OF_BOTTOM_LEFT_BLOCK_FILES = 1
BlockFile["bottom-left"] = {}
BlockFile["bottom-left"][1] = "res/Blocks/bottom-left.lua"

NUM_OF_BOTTOM_LEFT_CORNER_TOP_RIGHT_BLOCK_FILES = 1
BlockFile["bottom-left-corner-top-right"] = {}
BlockFile["bottom-left-corner-top-right"][1] = "res/Blocks/bottom-left-corner-top-right.lua"

NUM_OF_BOTTOM_RIGHT_BLOCK_FILES = 1
BlockFile["bottom-right"] = {}
BlockFile["bottom-right"][1] = "res/Blocks/bottom-right.lua"

NUM_OF_EMPTY_BLOCK_FILES = 1
BlockFile["empty"] = {}
BlockFile["empty"][1] = "res/Blocks/empty.lua"

NUM_OF_BOTTOM_RIGHT_CORNER_TOP_LEFT_BLOCK_FILES = 1
BlockFile["bottom-right-corner-top-left"] = {}
BlockFile["bottom-right-corner-top-left"][1] = "res/Blocks/bottom-right-corner-top-left.lua"

NUM_OF_CORNER_BOTTOM_BOTH_BLOCK_FILES = 1
BlockFile["corner-bottom-both"] = {}
BlockFile["corner-bottom-both"][1] = "res/Blocks/corner-bottom-both.lua"

NUM_OF_CORNER_BOTTOM_LEFT_BLOCK_FILES = 1
BlockFile["corner-bottom-left"] = {}
BlockFile["corner-bottom-left"][1] = "res/Blocks/corner-bottom-left.lua"

NUM_OF_CORNER_BOTTOM_RIGHT_BLOCK_FILES = 1
BlockFile["corner-bottom-right"] = {}
BlockFile["corner-bottom-right"][1] = "res/Blocks/corner-bottom-right.lua"

NUM_OF_CORNER_LEFT_BOTH_BLOCK_FILES = 1
BlockFile["corner-left-both"] = {}
BlockFile["corner-left-both"][1] = "res/Blocks/corner-left-both.lua"

NUM_OF_CORNER_RIGHT_BOTH_BLOCK_FILES = 1
BlockFile["corner-right-both"] = {}
BlockFile["corner-right-both"][1] = "res/Blocks/corner-right-both.lua"

NUM_OF_CORNER_TOP_BOTH_BLOCK_FILES = 1
BlockFile["corner-top-both"] = {}
BlockFile["corner-top-both"][1] = "res/Blocks/corner-top-both.lua"

NUM_OF_CORNER_TOP_LEFT_BLOCK_FILES = 1
BlockFile["corner-top-left"] = {}
BlockFile["corner-top-left"][1] = "res/Blocks/corner-top-left.lua"

NUM_OF_CORNER_TOP_RIGHT_BLOCK_FILES = 1
BlockFile["corner-top-right"] = {}
BlockFile["corner-top-right"][1] = "res/Blocks/corner-top-right.lua"

NUM_OF_EMPTY_BLOCK_FILES = 1
BlockFile["empty"] = {}
BlockFile["empty"][1] = "res/Blocks/empty.lua"

BlockFile["Official"] = {}
BlockFile["Official"]["Store"] = "res/Blocks/normal.lua"

NUM_OF_REGULAR_BLOCKS_FILES = 8
BlockFile["normal"] = {}
BlockFile["normal"][1] = "res/Blocks/normal.lua"
BlockFile["normal"][2] = "res/Blocks/normal.lua"
BlockFile["normal"][3] = "res/Blocks/normal.lua"
BlockFile["normal"][4] = "res/Blocks/normal.lua"
BlockFile["normal"][5] = "res/Blocks/normal.lua"
BlockFile["normal"][6] = "res/Blocks/normal.lua"
BlockFile["normal"][7] = "res/Blocks/normal.lua"
BlockFile["normal"][8] = "res/Blocks/normal.lua"