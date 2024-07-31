	#define  LEBAO_FONT_PADDING				(5)

	#define  LEBAO_LIST_HEIGHT				(90)
	#define  LEBAO_LIST_IMAGE_WIDTH			(80)
	#define  LEBAO_LIST_IMAGE_HEIGHT		(80)
	#define  LEBAO_LIST_IMAGE_WIDTH_SAMLL	(20)
	#define  LEBAO_LIST_ITEM_RIGHT			(310)

	#define  LEBAO_LEFT_PADDING_			(5)
	#define  LEBAO_TOP_PADDING_				(1)
	#define  LEBAO_RIGHT_PADDING_			(5)
	#define  LEBAO_BOTTOM_PADDING_			(2)

//|---------------------------|
//|  @     text               |
//|---------------------------|
//GUIITEM_STYLE_ONE_LINE_ICON_80X80_TEXT_FIXED_HEIGHT_90,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_icon_80x80_text_fixed_height_90 =
{
	58, 58,  //(LCD_HEIGHT - MMI_TITLE_THEME_HEIGHT - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT) / lebao_menu_max
	{
		{
			0,
			{ 2, 1,  58 + MMI_LIST_ICON_LEFT_SPACE,   58 - 1}, // normal
			{ 2, 1,  58 + MMI_LIST_ICON_LEFT_SPACE ,  58 - 1}, // focus
			0, 0,
			0, 0
		},
		{
			0,
			{ 58 + 2 + MMI_LIST_TEXT_LEFT_SPACE,  0,  LEBAO_LIST_ITEM_RIGHT - MMI_LIST_TEXT_LEFT_SPACE * 2, 58 - 1 },
			{ 58 + 2 + MMI_LIST_TEXT_LEFT_SPACE,  0,  LEBAO_LIST_ITEM_RIGHT - MMI_LIST_TEXT_LEFT_SPACE * 2, 58 - 1 },
			MMI_DEFAULT_NORMAL_FONT, MMI_DEFAULT_NORMAL_FONT,
			0, 100
		},
		{0},
		{0},
		{0},
		{0},
	},

	1, MMILIST_INVALID_ITEM_INDEX
};


//|---------------------------|
//|  @@     text              |
//|---------------------------|
//GUIITEM_STYLE_ONE_LINE_ICON_80X80_TEXT_FIXED_HEIGHT_90,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_icon_text_fixed_height_90 =
{
	58, 58,  //(LCD_HEIGHT - MMI_TITLE_THEME_HEIGHT - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT) / lebao_menu_max
	{
		{
			0,
			{ 2, 1,  58 + MMI_LIST_ICON_LEFT_SPACE,   58 - 1}, // normal
			{ 2, 1,  58 + MMI_LIST_ICON_LEFT_SPACE ,  58 - 1}, // focus
			0, 0,
			0, 0
		},
		{
			0,
			{ 58 + 2 + MMI_LIST_TEXT_LEFT_SPACE,  0,  LEBAO_LIST_ITEM_RIGHT - MMI_LIST_TEXT_LEFT_SPACE * 2, 58 - 1 },
			{ 58 + 2 + MMI_LIST_TEXT_LEFT_SPACE,  0,  LEBAO_LIST_ITEM_RIGHT - MMI_LIST_TEXT_LEFT_SPACE * 2, 58 - 1 },
			MMI_DEFAULT_NORMAL_FONT, MMI_DEFAULT_NORMAL_FONT,
			0, 100
		},
		{
			0,
			{ 3, 1,  32 + 3,  1 + 16}, // 32px, 16px
			{ 3, 1,  32 + 3,  1 + 16},
			0, 0,
			0, 0
		},

		{0},
		{0},
		{0},
	},

	1, MMILIST_INVALID_ITEM_INDEX
};


//|---------------------------|
//|  @     text               |
//|        text               |
//|---------------------------|
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_icon_20x72_2_text_text_fixed_height_90 =
{
	80, 80,
	{
		{
			0,
			{ 0, 1,  20 + MMI_LIST_ICON_LEFT_SPACE,  80 - 1}, // normal
			{ 0, 1,  20 + MMI_LIST_ICON_LEFT_SPACE,  80 - 1}, // focus
			0, 0,
			0, 0
		},
		{
			0,
			{ 25 + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  320 - 10, 80 / 2 - 1},
			{ 25 + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  320 - 10, 80 / 2 - 1},
			MMI_DEFAULT_NORMAL_FONT-2, MMI_DEFAULT_NORMAL_FONT-2,
			0, 1
		},
		{
			0,
			{ 25 + MMI_LIST_TEXT_LEFT_SPACE,  80 / 2, LEBAO_LIST_ITEM_RIGHT, 80 - MMI_LIST_ITEM_BOTTOM_MARGIN - 1},
			{ 25 + MMI_LIST_TEXT_LEFT_SPACE,  80 / 2, LEBAO_LIST_ITEM_RIGHT, 80 - MMI_LIST_ITEM_BOTTOM_MARGIN - 1},
			MMI_DEFAULT_SMALL_FONT, MMI_DEFAULT_SMALL_FONT, //SONG_FONT_16
			0, 1
		},
		{
			0,
			{ 25, 78, 320 - 10, 79}, //split line
			{ 25, 78, 320 - 10, 79},
			0, 0,
			0, 1
		},
		{0},
		{0},
	},

	1, MMILIST_INVALID_ITEM_INDEX
};

