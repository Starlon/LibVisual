#ifndef _LV_UI_H
#define _LV_UI_H

#include <libvisual/lv_list.h>
#include <libvisual/lv_param.h>
#include <libvisual/lv_video.h>
#include <libvisual/lv_common.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define VISUAL_UI_WIDGET(obj)				(VISUAL_CHECK_CAST ((obj), VISUAL_WIDGET_TYPE_WIDGET, VisUIWidget))
#define VISUAL_UI_CONTAINER(obj)			(VISUAL_CHECK_CAST ((obj), VISUAL_WIDGET_TYPE_CONTAINER, VisUIContainer))
#define VISUAL_UI_BOX(obj)				(VISUAL_CHECK_CAST ((obj), VISUAL_WIDGET_TYPE_BOX, VisUIBox))
#define VISUAL_UI_TABLE_ENTRY(obj)			(VISUAL_CHECK_CAST ((obj), VISUAL_WIDGET_TYPE_TABLE, VisUITableEntry))
#define VISUAL_UI_TABLE(obj)				(VISUAL_CHECK_CAST ((obj), VISUAL_WIDGET_TYPE_TABLE, VisUITable))
#define VISUAL_UI_FRAME(obj)				(VISUAL_CHECK_CAST ((obj), VISUAL_WIDGET_TYPE_FRAME, VisUIFrame))
#define VISUAL_UI_LABEL(obj)				(VISUAL_CHECK_CAST ((obj), VISUAL_WIDGET_TYPE_LABEL, VisUILabel))
#define VISUAL_UI_IMAGE(obj)				(VISUAL_CHECK_CAST ((obj), VISUAL_WIDGET_TYPE_IMAGE, VisUIImage))
#define VISUAL_UI_SEPARATOR(obj)			(VISUAL_CHECK_CAST ((obj), VISUAL_WIDGET_TYPE_SEPARATOR, VisUISeparator))
#define VISUAL_UI_MUTATOR(obj)				(VISUAL_CHECK_CAST ((obj), VISUAL_WIDGET_TYPE_MUTATOR, VisUIMutator))
#define VISUAL_UI_RANGE(obj)				(VISUAL_CHECK_CAST ((obj), VISUAL_WIDGET_TYPE_RANGE, VisUIRange))
#define VISUAL_UI_ENTRY(obj)				(VISUAL_CHECK_CAST ((obj), VISUAL_WIDGET_TYPE_ENTRY, VisUIEntry))
#define VISUAL_UI_SLIDER(obj)				(VISUAL_CHECK_CAST ((obj), VISUAL_WIDGET_TYPE_SLIDER, VisUISlider))
#define VISUAL_UI_NUMERIC(obj)				(VISUAL_CHECK_CAST ((obj), VISUAL_WIDGET_TYPE_NUMERIC, VisUINumeric))
#define VISUAL_UI_COLOR(obj)				(VISUAL_CHECK_CAST ((obj), VISUAL_WIDGET_TYPE_COLOR, VisUIColor))
#define VISUAL_UI_CHOICE_ENTRY(obj)			(VISUAL_CHECK_CAST ((obj), VISUAL_WIDGET_TYPE_CHOICE, VisUIChoiceEntry))
#define VISUAL_UI_CHOICE(obj)				(VISUAL_CHECK_CAST ((obj), VISUAL_WIDGET_TYPE_CHOICE, VisUIChoice))
#define VISUAL_UI_POPUP(obj)				(VISUAL_CHECK_CAST ((obj), VISUAL_WIDGET_TYPE_POPUP, VisUIPopup))
#define VISUAL_UI_LIST(obj)				(VISUAL_CHECK_CAST ((obj), VISUAL_WIDGET_TYPE_LIST, VisUIList))
#define VISUAL_UI_RADIO(obj)				(VISUAL_CHECK_CAST ((obj), VISUAL_WIDGET_TYPE_RADIO, VisUIRadio))
#define VISUAL_UI_CHECKBOX(obj)				(VISUAL_CHECK_CAST ((obj), VISUAL_WIDGET_TYPE_CHECKBOX, VisUICheckbox))

/**
 * Enumerate to define the different types of VisUIWidgets.
 */
typedef enum {
	VISUAL_WIDGET_TYPE_NULL,	/**< NULL widget */
	VISUAL_WIDGET_TYPE_WIDGET,	/**< Base widget: \a VisUIWidget. */
	VISUAL_WIDGET_TYPE_CONTAINER,	/**< Container widget: \a VisUIContainer. */
	VISUAL_WIDGET_TYPE_BOX,		/**< Box widget: \a VisUIBox. */
	VISUAL_WIDGET_TYPE_TABLE,	/**< Table widget: \a VisUITable. */
	VISUAL_WIDGET_TYPE_FRAME,	/**< Frame widget: \a VisUIFrame. */
	VISUAL_WIDGET_TYPE_LABEL,	/**< Label widget: \a VisUILabel. */
	VISUAL_WIDGET_TYPE_IMAGE,	/**< Image widget: \a VisUIImage. */
	VISUAL_WIDGET_TYPE_SEPARATOR,	/**< Separator widget: \a VisUISeparator. */
	VISUAL_WIDGET_TYPE_MUTATOR,	/**< Mutator base widget: \a VisUIMutator. */
	VISUAL_WIDGET_TYPE_RANGE,	/**< Range base widget: \a VisUIRange. */
	VISUAL_WIDGET_TYPE_ENTRY,	/**< Entry box widget: \a VisUIEntry. */
	VISUAL_WIDGET_TYPE_SLIDER,	/**< Slider widget: \a VisUISlider. */
	VISUAL_WIDGET_TYPE_NUMERIC,	/**< Numeric widget: \a VisUINumeric. */
	VISUAL_WIDGET_TYPE_COLOR,	/**< Color widget: \a VisUIColor. */
	VISUAL_WIDGET_TYPE_CHOICE,	/**< Choice base widget: \a VisUIChoice. */
	VISUAL_WIDGET_TYPE_POPUP,	/**< Popup widget: \a VisUIPopup. */
	VISUAL_WIDGET_TYPE_LIST,	/**< List widget: \a VisUIList. */
	VISUAL_WIDGET_TYPE_RADIO,	/**< Radio widget: \a VisUIRadio. */
	VISUAL_WIDGET_TYPE_CHECKBOX	/**< Checkbox widget: \a VisUICheckbox. */
} VisUIWidgetType;

/**
 * Enumerate to define the different types of widget orientation. This is used
 * by a few widgets that can be aligned both vertical and horizontal.
 */
typedef enum {
	VISUAL_ORIENT_TYPE_NONE,	/**< No orientation, use the default. */
	VISUAL_ORIENT_TYPE_HORIZONTAL,	/**< Horizontal orientation. */
	VISUAL_ORIENT_TYPE_VERTICAL	/**< Vertical orientation. */
} VisUIOrientType;

typedef struct _VisUIWidget VisUIWidget;
typedef struct _VisUIContainer VisUIContainer;
typedef struct _VisUIBox VisUIBox;
typedef struct _VisUITableEntry VisUITableEntry;
typedef struct _VisUITable VisUITable;
typedef struct _VisUIFrame VisUIFrame;
typedef struct _VisUILabel VisUILabel;
typedef struct _VisUIImage VisUIImage;
typedef struct _VisUISeparator VisUISeparator;
typedef struct _VisUIMutator VisUIMutator;
typedef struct _VisUIRange VisUIRange;
typedef struct _VisUIEntry VisUIEntry;
typedef struct _VisUISlider VisUISlider;
typedef struct _VisUINumeric VisUINumeric;
typedef struct _VisUIColor VisUIColor;
typedef struct _VisUIChoiceList VisUIChoiceList;
typedef struct _VisUIChoiceEntry VisUIChoiceEntry;
typedef struct _VisUIChoice VisUIChoice;
typedef struct _VisUIPopup VisUIPopup;
typedef struct _VisUIList VisUIList;
typedef struct _VisUIRadio VisUIRadio;
typedef struct _VisUICheckbox VisUICheckbox;

/* FIXME, fix the links, they are screwed up because of the typedefs, there is some way around it
 * but hey. */
/**
 * 
 * The super class for al VisUIWidgets. All the typical VisUIWidgets
 * derive from this. VisUIWidget is used as an intermediate user interface
 * description. Mainly to set up configuration dialogs for plugins that
 * are not widget set dependant.
 *
 * The VisUIWidget class hierarchy looks like following:
 * - \a _VisUIWidget
 *	- \a VisUILabel
 *	- \a VisUIImage
 *	- \a VisUIContainer
 *		- \a VisUIBox
 *		- \a VisUITable
 *		- \a VisUIFrame
 *	- \a VisUIMutator
 *		- \a VisUIText
 *		- \a VisUIColor
 *		- \a VisUIRange
 *			- \a VisUISlider
 *			- \a VisUINumeric
 *		- \a VisUIChoice
 *			- \a VisUIPopup
 *			- \a VisUIList
 *			- \a VisUIRadio
 *			- \a VisUICheckbox
 */
struct _VisUIWidget {
	VisObject		 object;	/**< The VisObject data. */

	VisUIWidget		*parent;	/**< Parent in which this VisUIWidget is packed.
						 * This is possibly NULL. */
	
	VisUIWidgetType		 type;		/**< Type of VisUIWidget. */

	const char		*tooltip;	/**< Optional tooltip text, this can be used to
						 * give the user some extra explanation about the
						 * user interface. */

	int			 width;		/**< When size requisition is used, the width value will
						 * be stored in this. */
	int			 height;	/**< When size requisition is used. the height value will
						 * be stored in this. When size requisition is not being
						 * done both width and height will contain of -1. */
};

/**
 * The VisUIContainer is a VisUIWidget that is used to pack other widgets in.
 * A basic container can contain just one VisUIWidget, however when VisUIBox or
 * VisUITable is used, it's possible to add more elements.
 */
struct _VisUIContainer {
	VisUIWidget		 widget;	/**< The VisUIWidget data. */

	VisUIWidget		*child;		/**< Pointer to the child VisUIWidget that is packed in this VisUIContainer. */
};

/**
 * The VisUIBox inherents from VisUIContainer, but is capable to contain more childeren.
 * The VisUIBox is used as a box of VisUIWidgets, packed vertical or horizontal.
 */
struct _VisUIBox {
	VisUIContainer		 container;	/**< The VisUIContainer data. */

	VisUIOrientType		 orient;	/**< Orientation, whatever the box packs the item in a vertical
						 * order or in a horizontal order. */

	VisList			 childs;	/**< A list of all child VisUIWidgets. */
};

/**
 * VisUITableEntry is an entry in a VisUITable. the VisUITableEntry is not a VisUIWidget on
 * itself. Instead it rembers the cell in which a VisUIWidget is placed in VisUITable and
 * also has a reference to the VisUIWidget.
 */
struct _VisUITableEntry {
	VisObject		object;		/**< The VisObject data. */

	int			row;		/**< Row to which the VisUITableEntry is associated. */
	int			col;		/**< Column to which the VisUITableEntry is associated. */

	VisUIWidget		*widget;	/**< The VisUIWidget that is connected to this entry in the VisUITable. */
};

/**
 * The VisUITable inherents from VisUIContainer, but is capable of placing VisUIWidgets in an aligned grid.
 */
struct _VisUITable {
	VisUIContainer		 container;	/**< The VisUIContainer data. */

	int			 rows;		/**< The number of rows in this VisUITable. */
	int			 cols;		/**< The number of columns in this VisUITable. */

	VisList			 childs;	/**< A list of all VisUITableEntry items that are related to
						 * this table. */
};

/**
 * The VisUIFrame inherents from VisUIContainer, it's used to put a frame with a label around a VisUIWidget.
 */
struct _VisUIFrame {
	VisUIContainer		 container;	/**< The VisUIContainer data. */

	const char		*name;		/**< The frame label text. */
};

/**
 * The VisUILabel inherents from a VisUIWidget, it's used to as a label item in the user interface.
 */
struct _VisUILabel {
	VisUIWidget		 widget;	/**< The VisUIWidget data. */

	const char		*text;		/**< The label text. */
	int			 bold;		/**< Whatever the label is being printed in bold or not. */
};

/**
 * The VisUIImage inherents from a VisUIWidget, it's used to display a VisVideo within the user interface. For
 * example it can be used to show a picture.
 */
struct _VisUIImage {
	VisUIWidget		 widget;	/**< The VisUIWidget data. */

	const VisVideo		*image;		/**< The VisUIImage containing the image data. */
};

/**
 * The VisUISeparator inherents from a VisUIWidget, it's used to display a separator between different user interface
 * elements.
 */
struct _VisUISeparator {
	VisUIWidget		 widget;	/**< The VisUIWidget data. */

	VisUIOrientType		 orient;	/**< The orientation, whatever the separator is drawn in vertical
						 * or horizontal style. */
};

/**
 * The VisUIMutator inherents from a VisUIWidget, it's used as a super class for different type of mutator VisUIWidgets.
 * Mutator VisUIWidgets are used to change a value in a VisParamEntry.
 */
struct _VisUIMutator {
	VisUIWidget		 widget;	/**< The VisUIWidget data. */

	const VisParamEntry	*param;		/**< The VisParamEntry parameter that is associated with this
						 * VisUIMutator. */
};

/**
 * The VisUIRange inherents from a VisUIMutator, it's a type of mutator widget that focus on numeric input and
 * numeric adjustment within a range.
 */
struct _VisUIRange {
	VisUIMutator		 mutator;	/**< The VisUIMutator data. */

	double			 min;		/**< The minimal value. */
	double			 max;		/**< The maximal value. */
	double			 step;		/**< Increase / decrease steps. */

	int			 precision;	/**< Precision, in the fashion of how many numbers behind
						 * the point. */
};

/**
 * The VisUIEntry inherents from a VisUIMutator, it's used as a way to input text.
 */
struct _VisUIEntry {
	VisUIMutator		 mutator;	/**< The VisUIMutator data. */

	int			 length;	/**< Maximal length for the text. */
};

/**
 * The VisUISlider inherents from a VisUIRange, it's used to display a slider which can be used
 * to adjust a numeric value.
 */
struct _VisUISlider {
	VisUIRange		 range;		/**< The VisUIRange data. */

	int			 showvalue;	/**< Don't show just the slider, but also the  value it
						 * represents at it's current position. */
};

/**
 * The VisUINumeric inherents from a VisUIRange, it's used to display a numeric spin button that
 * can be used to adjust a numeric value. A numeric spin button contains out of a small text field
 * displaying the actual value, followed by two buttons which are used to increase or decrease the value.
 *
 * In most VisUI native implementations it's also possible to adjust the text field directly using
 * the keyboard.
 */
struct _VisUINumeric {
	VisUIRange		 range;		/**< The VisUIRange data. */
};

/**
 * The VisUIColor inherents from a VisUIMutator, it's used to adjust the color that is encapsulated by
 * a VisParamEntry.
 */
struct _VisUIColor {
	VisUIMutator		 mutator;	/**< The VisUIMutator data. */
};

/**
 * The VisUIChoiceList is not a VisUIWidget, but it's used by the different types of VisUIChoice widgets to
 * store information about choices.
 */
struct _VisUIChoiceList {
	VisObject		 object;	/**< The VisObject data. */

	int			 count;		/**< The amount of choices that are present. */
	VisList			 choices;	/**< A list of VisUIChoiceEntry elements. */ 
};

/**
 * The VisUIChoiceEntry is not a VisUIWidget, but it's used by the different types of VisUIChoice widgets to
 * store information regarding a choice within a VisUIChoiceList.
 */
struct _VisUIChoiceEntry {
	VisObject		 object;	/**< The VisObject data. */

	const char		*name;		/**< Name of this VisChoiceEntry. */
	
	const VisParamEntry	*value;		/**< Link to the VisParamEntry that contains the value
						 * for this VisChoiceEntry. */
};

/**
 * The VisUIChoice inherents from a VisUIMutator, it's used as a super class for the different types of
 * VisUIChoice VisUIWidgets.
 */
struct _VisUIChoice {
	VisUIMutator		 mutator;	/**< The VisUIMutator data. */

	const VisParamEntry	*param;		/**< Pointer to the VisParamEntry that is the target to
						 * contain the actual value. */

	VisUIChoiceList		 choices;	/**< The different choices present. */
};

/**
 * The VisUIPopup inherents from a VisUIChoice, it's used to represent choices in a popup fashion, where you
 * can select an item.
 */
struct _VisUIPopup {
	VisUIChoice		 choice;	/**< The VisUIChoice data. */
};

/**
 * The VisUIList inherents from a VisUIChoice, it's used to represent choices in a list fashion.
 */
struct _VisUIList {
	VisUIChoice		 choice;	/**< The VisUIChoice data. */
};

/**
 * The VisUIRadio inherents from a VisUIChoice, it's used to represent choices in the fashion of radio buttons. These
 * are a grouped type of checkboxes where only one item can be active at once.
 */
struct _VisUIRadio {
	VisUIChoice		 choice;	/**< The VisUIChoice data. */

	VisUIOrientType		 orient;	/**< Orientation of how the different radio buttons in the VisUIRadio button
						 * group is ordered. */
};

/**
 * The VisUICheckbox inherents from a VisUIChoice, it's used to represent one single checkbox widget. For this reason it
 * can only represent two choices. One for the toggled state and one for the untoggled state.
 */
struct _VisUICheckbox {
	VisUIChoice		 choice;	/**< The VisUIChoice data. */

	const char		*name;		/**< Optional text behind the textbox. */
};

/* prototypes */
VisUIWidget *visual_ui_widget_new (void);
int visual_ui_widget_set_size_request (VisUIWidget *widget, int width, int height);
int visual_ui_widget_set_tooltip (VisUIWidget *widget, const char *tooltip);
const char *visual_ui_widget_get_tooltip (VisUIWidget *widget);
VisUIWidget *visual_ui_widget_get_top (VisUIWidget *widget);
VisUIWidget *visual_ui_widget_get_parent (VisUIWidget *widget);
VisUIWidgetType visual_ui_widget_get_type (VisUIWidget *widget);

int visual_ui_container_add (VisUIContainer *container, VisUIWidget *widget);
VisUIWidget *visual_ui_container_get_child (VisUIContainer *container);

VisUIWidget *visual_ui_box_new (VisUIOrientType orient);
int visual_ui_box_pack (VisUIBox *box, VisUIWidget *widget);
VisList *visual_ui_box_get_childs (VisUIBox *box);
VisUIOrientType visual_ui_box_get_orient (VisUIBox *box);

VisUIWidget *visual_ui_table_new (int rows, int cols);
VisUITableEntry *visual_ui_table_entry_new (VisUIWidget *widget, int row, int col);
int visual_ui_table_attach (VisUITable *table, VisUIWidget *widget, int row, int col);
VisList *visual_ui_table_get_childs (VisUITable *table);

VisUIWidget *visual_ui_frame_new (const char *name);

VisUIWidget *visual_ui_label_new (const char *text, int bold);
int visual_ui_label_set_text (VisUILabel *label, const char *text);
int visual_ui_label_set_bold (VisUILabel *label, int bold);
const char *visual_ui_label_get_text (VisUILabel *label);

VisUIWidget *visual_ui_image_new (const VisVideo *video);
int visual_ui_image_set_video (VisUIImage *image, const VisVideo *video);
const VisVideo *visual_ui_image_get_video (VisUIImage *image);

VisUIWidget *visual_ui_separator_new (VisUIOrientType orient);
VisUIOrientType visual_ui_separator_get_orient (VisUISeparator *separator);

int visual_ui_mutator_set_param (VisUIMutator *mutator, const VisParamEntry *param);
const VisParamEntry *visual_ui_mutator_get_param (VisUIMutator *mutator);

int visual_ui_range_set_properties (VisUIRange *range, double min, double max, double step, int precision);
int visual_ui_range_set_max (VisUIRange *range, double max);
int visual_ui_range_set_min (VisUIRange *range, double min);
int visual_ui_range_set_step (VisUIRange *range, double step);
int visual_ui_range_set_precision (VisUIRange *range, int precision);

VisUIWidget *visual_ui_entry_new (void);
int visual_ui_entry_set_length (VisUIEntry *entry, int length);

VisUIWidget *visual_ui_slider_new (int showvalue);

VisUIWidget *visual_ui_numeric_new (void);

VisUIWidget *visual_ui_color_new (void);

VisUIChoiceEntry *visual_ui_choice_entry_new (const char *name, const VisParamEntry *value);
int visual_ui_choice_add (VisUIChoice *choice, const char *name, const VisParamEntry *value);
int visual_ui_choice_add_many (VisUIChoice *choice, const VisParamEntry *paramchoices);
int visual_ui_choice_free_choices (VisUIChoice *choice);
int visual_ui_choice_set_active (VisUIChoice *choice, int index);
int visual_ui_choice_get_active (VisUIChoice *choice);
VisUIChoiceEntry *visual_ui_choice_get_choice (VisUIChoice *choice, int index);
VisUIChoiceList *visual_ui_choice_get_choices (VisUIChoice *choice);

/* FIXME look at lists with multiple selections... */

VisUIWidget *visual_ui_popup_new (void);

VisUIWidget *visual_ui_list_new (void);

VisUIWidget *visual_ui_radio_new (VisUIOrientType orient);

VisUIWidget *visual_ui_checkbox_new (const char *name, int boolcheck);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _LV_UI_H */
