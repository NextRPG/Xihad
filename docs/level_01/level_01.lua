return {
  version = "1.1",
  luaversion = "5.1",
  orientation = "orthogonal",
  width = 15,
  height = 17,
  tilewidth = 32,
  tileheight = 32,
  properties = {},
  tilesets = {
    {
      name = "山崖",
      firstgid = 1,
      tilewidth = 32,
      tileheight = 32,
      spacing = 0,
      margin = 0,
      image = "cliff.png",
      imagewidth = 32,
      imageheight = 32,
      properties = {},
      tiles = {}
    },
    {
      name = "texture",
      firstgid = 2,
      tilewidth = 32,
      tileheight = 32,
      spacing = 0,
      margin = 0,
      image = "texture.png",
      imagewidth = 96,
      imageheight = 32,
      properties = {},
      tiles = {
        {
          id = 0,
          properties = {
            ["terrain"] = "1"
          }
        },
        {
          id = 1,
          properties = {
            ["terrain"] = "2"
          }
        },
        {
          id = 2,
          properties = {
            ["terrain"] = "3"
          }
        }
      }
    },
    {
      name = "character",
      firstgid = 5,
      tilewidth = 32,
      tileheight = 32,
      spacing = 0,
      margin = 0,
      image = "character.png",
      imagewidth = 32,
      imageheight = 70,
      properties = {},
      tiles = {
        {
          id = 0,
          properties = {
            ["maxHP"] = "100"
          }
        }
      }
    },
    {
      name = "物品",
      firstgid = 7,
      tilewidth = 32,
      tileheight = 23,
      spacing = 0,
      margin = 0,
      image = "treasure.png",
      imagewidth = 32,
      imageheight = 23,
      properties = {},
      tiles = {}
    }
  },
  layers = {
    {
      type = "tilelayer",
      name = "map",
      x = 0,
      y = 0,
      width = 15,
      height = 17,
      visible = true,
      opacity = 1,
      properties = {},
      encoding = "lua",
      data = {
        1, 1, 1, 1, 1, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 4,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 4, 4,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 4, 4, 4,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 4, 4, 4,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 4, 4, 4,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 4, 4, 4,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 4, 4, 4,
        3, 3, 3, 3, 3, 3, 2, 2, 3, 3, 3, 3, 3, 3, 3,
        3, 3, 3, 3, 3, 3, 2, 2, 3, 3, 3, 3, 3, 3, 3,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 4, 4, 4,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 4, 4, 4,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 4, 4, 4, 4,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 4, 4, 4, 4
      }
    },
    {
      type = "objectgroup",
      name = "AIs",
      visible = true,
      opacity = 1,
      properties = {},
      objects = {
        {
          name = "",
          type = "",
          shape = "rectangle",
          x = 224,
          y = 320,
          width = 0,
          height = 0,
          gid = 6,
          visible = true,
          properties = {}
        },
        {
          name = "",
          type = "",
          shape = "rectangle",
          x = 321,
          y = 256,
          width = 0,
          height = 0,
          gid = 6,
          visible = true,
          properties = {}
        },
        {
          name = "",
          type = "",
          shape = "rectangle",
          x = 161,
          y = 257,
          width = 0,
          height = 0,
          gid = 6,
          visible = true,
          properties = {}
        },
        {
          name = "",
          type = "",
          shape = "rectangle",
          x = 289,
          y = 96,
          width = 0,
          height = 0,
          gid = 6,
          visible = true,
          properties = {}
        },
        {
          name = "",
          type = "",
          shape = "rectangle",
          x = 161,
          y = 96,
          width = 0,
          height = 0,
          gid = 6,
          visible = true,
          properties = {}
        }
      }
    },
    {
      type = "objectgroup",
      name = "heros",
      visible = true,
      opacity = 1,
      properties = {},
      objects = {
        {
          name = "",
          type = "",
          shape = "rectangle",
          x = 129,
          y = 481,
          width = 0,
          height = 0,
          gid = 5,
          visible = true,
          properties = {}
        },
        {
          name = "",
          type = "",
          shape = "rectangle",
          x = 193,
          y = 481,
          width = 0,
          height = 0,
          gid = 5,
          visible = true,
          properties = {}
        },
        {
          name = "",
          type = "",
          shape = "rectangle",
          x = 257,
          y = 480,
          width = 0,
          height = 0,
          gid = 5,
          visible = true,
          properties = {}
        }
      }
    },
    {
      type = "objectgroup",
      name = "elements",
      visible = true,
      opacity = 1,
      properties = {},
      objects = {
        {
          name = "",
          type = "",
          shape = "rectangle",
          x = 448,
          y = 160,
          width = 0,
          height = 0,
          gid = 7,
          visible = true,
          properties = {}
        },
        {
          name = "",
          type = "",
          shape = "rectangle",
          x = 978,
          y = 363,
          width = 0,
          height = 0,
          gid = 4,
          visible = true,
          properties = {}
        }
      }
    }
  }
}
