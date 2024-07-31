	#define  LEBAO_FONT_PADDING				(3)

	#define  LEBAO_LIST_HEIGHT				(42)
	#define  LEBAO_LIST_IMAGE_WIDTH			(36)
	#define  LEBAO_LIST_IMAGE_HEIGHT		(36)
	#define  LEBAO_LIST_IMAGE_WIDTH_SAMLL	(10)
	#define  LEBAO_LIST_ITEM_RIGHT			(122)

	#define  LEBAO_LEFT_PADDING_			(2)
	#define  LEBAO_TOP_PADDING_				(2)
	#define  LEBAO_RIGHT_PADDING_			(2)
	#define  LEBAO_BOTTOM_PADDING_			(2)
	
//|---------------------------|
//|  @     text               |
//|---------------------------|
  //GUIITEM_STYLE_ONE_LINE_ICON_80X80_TEXT_FIXED_HEIGHT_90,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_icon_80x80_text_fixed_height_90 =
{
	42, 42,
	{
		{
			0,
			{ LEBAO_LEFT_PADDING_ + LEBAO_ROUND_PADDING_, 1,  36 + LEBAO_LEFT_PADDING_ + LEBAO_ROUND_PADDING_,   42 - 1}, // normal
			{ LEBAO_LEFT_PADDING_ + LEBAO_ROUND_PADDING_, 1,  36 + LEBAO_LEFT_PADDING_ + LEBAO_ROUND_PADDING_ ,  42 - 1}, // focus
			0, 0,
			0, 0
		},
		{
			0,
			{ 36 + LEBAO_LEFT_PADDING_ * 2 + LEBAO_ROUND_PADDING_,  0,  LEBAO_LIST_ITEM_RIGHT - LEBAO_ROUND_PADDING_, 42 - 1 },
			{ 36 + LEBAO_LEFT_PADDING_ * 2 + LEBAO_ROUND_PADDING_,  0,  LEBAO_LIST_ITEM_RIGHT - LEBAO_ROUND_PADDING_, 42 - 1 },
			MMI_DEFAULT_NORMAL_FONT, MMI_DEFAULT_NORMAL_FONT,
			0, 100
		},
		{0},
		{0},
		{0},
		{0},
	},

	0, MMILIST_INVALID_ITEM_INDEX
};

//|---------------------------|
//|  @@     text              |
//|---------------------------|
//GUIITEM_STYLE_ONE_LINE_ICON_80X80_TEXT_FIXED_HEIGHT_90,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_icon_text_fixed_height_90 =
{
	42, 42,
	{
		{
			0,
			{ LEBAO_LEFT_PADDING_ + LEBAO_ROUND_PADDING_, 1,  36 + LEBAO_LEFT_PADDING_ + LEBAO_ROUND_PADDING_,  42 - 1}, // normal
			{ LEBAO_LEFT_PADDING_ + LEBAO_ROUND_PADDING_, 1,  36 + LEBAO_LEFT_PADDING_ + LEBAO_ROUND_PADDING_,  42 - 1}, // focus
			0, 0,
			0, 0
		},
		{
			0,
			{ 36 + LEBAO_LEFT_PADDING_ * 2 + LEBAO_ROUND_PADDING_,  0,  MMI_LIST_ITEM_RIGHT - LEBAO_ROUND_PADDING_, 42 - 1 },
			{ 36 + LEBAO_LEFT_PADDING_ * 2 + LEBAO_ROUND_PADDING_,  0,  MMI_LIST_ITEM_RIGHT - LEBAO_ROUND_PADDING_, 42 - 1 },
			MMI_DEFAULT_NORMAL_FONT, MMI_DEFAULT_NORMAL_FONT,
			0, 50
		},
		{
			0,
			{ LEBAO_LEFT_PADDING_ + LEBAO_ROUND_PADDING_, 0,  16 + LEBAO_LEFT_PADDING_ + LEBAO_ROUND_PADDING_,  0 + 16}, // 16px, 16px
			{ LEBAO_LEFT_PADDING_ + LEBAO_ROUND_PADDING_, 0,  16 + LEBAO_LEFT_PADDING_ + LEBAO_ROUND_PADDING_,  0 + 16},
			0, 0,
			0, 0
		},
		{0},
		{0},
		{0},
	},

	0, MMILIST_INVALID_ITEM_INDEX
};


//|---------------------------|
//|  @     text               |
//|        text               |
//|---------------------------|
// icon : 10 X 36
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_icon_20x72_2_text_text_fixed_height_90 =
{
	42, 42,
	{
		{
			0,
			{ 2 + LEBAO_ROUND_PADDING_, 1,  10 + LEBAO_LEFT_PADDING_ + LEBAO_ROUND_PADDING_,   42 - 1}, // normal
			{ 2 + LEBAO_ROUND_PADDING_, 1,  10 + LEBAO_LEFT_PADDING_ + LEBAO_ROUND_PADDING_ ,  42 - 1}, // focus
			0, 0,
			0, 0
		},
		{
			0,
			{ 10 + LEBAO_LEFT_PADDING_ + LEBAO_ROUND_PADDING_,  LEBAO_TOP_PADDING_,  LEBAO_LIST_ITEM_RIGHT - LEBAO_ROUND_PADDING_, 42 / 2 - 1},
			{ 10 + LEBAO_LEFT_PADDING_ + LEBAO_ROUND_PADDING_,  LEBAO_TOP_PADDING_,  LEBAO_LIST_ITEM_RIGHT - LEBAO_ROUND_PADDING_, 42 / 2 - 1},
			MMI_DEFAULT_NORMAL_FONT, MMI_DEFAULT_NORMAL_FONT,
			0, 100
		},
		{
			0,
			{ 10 + LEBAO_LEFT_PADDING_ + LEBAO_ROUND_PADDING_,  42 / 2, LEBAO_LIST_ITEM_RIGHT - LEBAO_ROUND_PADDING_, 42 - LEBAO_BOTTOM_PADDING_},
			{ 10 + LEBAO_LEFT_PADDING_ + LEBAO_ROUND_PADDING_,  42 / 2, LEBAO_LIST_ITEM_RIGHT - LEBAO_ROUND_PADDING_, 42 - LEBAO_BOTTOM_PADDING_},
			MMI_DEFAULT_SMALL_FONT, MMI_DEFAULT_SMALL_FONT, //SONG_FONT_16
			0, 100
		},
		{
			0,
			{ 5 + LEBAO_ROUND_PADDING_, 43,  5 + 115 - LEBAO_ROUND_PADDING_, 42}, //split line
			{ 5 + LEBAO_ROUND_PADDING_, 43,  5 + 115 - LEBAO_ROUND_PADDING_, 42},
			0, 0,
			0, 1
		},
		{0},
		{0},
	},

	1, MMILIST_INVALID_ITEM_INDEX
};

//|---------------------------|
//|  @     text               |
//|---------------------------|
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_icon_20x40_1_text_fixed_height_90 =
{
	40, 40,
	{
		{
			0,
			{ 0, 1,  20 + MMI_LIST_ICON_LEFT_SPACE,  40 - 1}, // normal
			{ 0, 1,  20 + MMI_LIST_ICON_LEFT_SPACE,  40 - 1}, // focus
			0, 0,
			0, 0
		},
		{
			0,
			{ 25 + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, 40 - 1},
			{ 25 + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, 40 - 1},
			MMI_DEFAULT_BIG_FONT-2, MMI_DEFAULT_BIG_FONT-2,
			0, 100
		},
		{0},
		{0},
		{0},
		{0},
	},

	1, MMILIST_INVALID_ITEM_INDEX
};
