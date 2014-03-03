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
      name = "texture",
      firstgid = 1,
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
      firstgid = 4,
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
        2, 2, 2, 2, 2, 2, 1, 1, 1, 2, 2, 2, 2, 2, 2,
        2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 3, 3, 3,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3,
        2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3
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
          name = "a",
          type = "",
          shape = "rectangle",
          x = 224,
          y = 320,
          width = 0,
          height = 0,
          gid = 5,
          visible = true,
          properties = {}
        },
        {
          name = "b",
          type = "",
          shape = "rectangle",
          x = 321,
          y = 256,
          width = 0,
          height = 0,
          gid = 5,
          visible = true,
          properties = {}
        },
        {
          name = "c",
          type = "",
          shape = "rectangle",
          x = 161,
          y = 257,
          width = 0,
          height = 0,
          gid = 5,
          visible = true,
          properties = {}
        },
        {
          name = "d",
          type = "",
          shape = "rectangle",
          x = 288,
          y = 320,
          width = 0,
          height = 0,
          gid = 5,
          visible = true,
          properties = {}
        },
        {
          name = "e",
          type = "",
          shape = "rectangle",
          x = 128,
          y = 320,
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
      name = "heros",
      visible = true,
      opacity = 1,
      properties = {},
      objects = {
        {
          name = "1",
          type = "",
          shape = "rectangle",
          x = 129,
          y = 481,
          width = 0,
          height = 0,
          gid = 4,
          visible = true,
          properties = {}
        },
        {
          name = "2",
          type = "",
          shape = "rectangle",
          x = 193,
          y = 481,
          width = 0,
          height = 0,
          gid = 4,
          visible = true,
          properties = {}
        },
        {
          name = "3",
          type = "",
          shape = "rectangle",
          x = 257,
          y = 480,
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
