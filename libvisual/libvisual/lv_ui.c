#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "lv_log.h"
#include "lv_ui.h"


static int widget_free (void *ptr);
static void list_widget_destroy (void *ptr);
static int widget_destroy (void *ptr);

static void table_entry_destroyer (void *ptr);


static void table_entry_destroyer (void *ptr)
{
	VisUITableEntry *tentry = ptr;

	visual_log_return_if_fail (tentry != NULL);
	
	widget_destroy (tentry->widget);
}

static int widget_free (void *ptr)
{
	VisUIWidget *widget = ptr;

	visual_log_return_val_if_fail (widget != NULL, -VISUAL_ERROR_UI_WIDGET_NULL);

	switch (widget->type) {
		case VISUAL_WIDGET_TYPE_NULL:
			return visual_mem_free (widget);

			break;

		case VISUAL_WIDGET_TYPE_WIDGET:
			return visual_mem_free (widget);

			break;

		case VISUAL_WIDGET_TYPE_BOX:
			return visual_ui_box_free (VISUAL_UI_BOX (widget));

			break;

		case VISUAL_WIDGET_TYPE_TABLE:
			return visual_ui_table_free (VISUAL_UI_TABLE (widget));

			break;
		
		case VISUAL_WIDGET_TYPE_FRAME:
			return visual_ui_frame_free (VISUAL_UI_FRAME (widget));

			break;

		case VISUAL_WIDGET_TYPE_LABEL:
			return visual_ui_label_free (VISUAL_UI_LABEL (widget));

			break;

		case VISUAL_WIDGET_TYPE_IMAGE:
			return visual_ui_image_free (VISUAL_UI_IMAGE (widget));

			break;

		case VISUAL_WIDGET_TYPE_SEPARATOR:
			return visual_ui_separator_free (VISUAL_UI_SEPARATOR (widget));

			break;

		case VISUAL_WIDGET_TYPE_ENTRY:
			return visual_ui_entry_free (VISUAL_UI_ENTRY (widget));

			break;

		case VISUAL_WIDGET_TYPE_SLIDER:
			return visual_ui_slider_free (VISUAL_UI_SLIDER (widget));

			break;

		case VISUAL_WIDGET_TYPE_NUMERIC:
			return visual_ui_numeric_free (VISUAL_UI_NUMERIC (widget));

			break;

		case VISUAL_WIDGET_TYPE_COLOR:
			return visual_ui_color_free (VISUAL_UI_COLOR (widget));

			break;

		case VISUAL_WIDGET_TYPE_POPUP:
			return visual_ui_popup_free (VISUAL_UI_POPUP (widget));

			break;

		case VISUAL_WIDGET_TYPE_LIST:
			return visual_ui_list_free (VISUAL_UI_LIST (widget));

			break;

		case VISUAL_WIDGET_TYPE_RADIO:
			return visual_ui_radio_free (VISUAL_UI_RADIO (widget));

			break;

		case VISUAL_WIDGET_TYPE_CHECKBOX:
			return visual_ui_checkbox_free (VISUAL_UI_CHECKBOX (widget));

			break;

		default:
			visual_log (VISUAL_LOG_CRITICAL, "Trying to free an unknown VisUI widget type");
			
			return -VISUAL_ERROR_UI_INVALID_TYPE;

			break;
	}

	return -VISUAL_ERROR_IMPOSSIBLE;
}

static void list_widget_destroy (void *ptr)
{
	widget_destroy (ptr);
}

static int widget_destroy (void *ptr)
{
	VisUIWidget *widget = ptr;

	visual_log_return_val_if_fail (widget != NULL, -VISUAL_ERROR_UI_WIDGET_NULL);

	switch (widget->type) {
		case VISUAL_WIDGET_TYPE_BOX:
			return visual_ui_box_destroy (VISUAL_UI_BOX (widget));

			break;

		case VISUAL_WIDGET_TYPE_TABLE:
			return visual_ui_table_destroy (VISUAL_UI_TABLE (widget));

			break;
		
		case VISUAL_WIDGET_TYPE_FRAME:
			return visual_ui_frame_destroy (VISUAL_UI_FRAME (widget));

			break;

		default:
			/* The others are the same destroy/free wise. */
			return widget_free (widget);
		
			break;
	}

	return -VISUAL_ERROR_IMPOSSIBLE;
}

/**
 * @defgroup VisUI VisUI
 * @{
 */

/**
 * Creates a new VisUIWidget structure.
 *
 * @return A newly allocated VisUIWidget, or NULL on error.
 */
VisUIWidget *visual_ui_widget_new ()
{
	VisUIWidget *widget;

	widget = visual_mem_new0 (VisUIWidget, 1);
	widget->type = VISUAL_WIDGET_TYPE_NULL;

	visual_ui_widget_set_size_request (VISUAL_UI_WIDGET (widget), -1, -1);

	return widget;
}

/**
 * Frees the VisUIWidget. This frees a VisUIWidget, when freeing a widget always
 * use this function, it will automaticly call the right internal function.
 *
 * @param widget Pointer to the VisUIWidget that needs to be freed.
 * 
 * @return VISUAL_OK on succes, or errors that can be thrown by any of the free functions.
 */
int visual_ui_widget_free (VisUIWidget *widget)
{
	visual_log_return_val_if_fail (widget != NULL, -VISUAL_ERROR_UI_WIDGET_NULL);

	return widget_free (widget);
}

/**
 * Destroys the VisUIWidget. This destroys a VisUIWidget and all it's childeren.
 * This function should be used to destroy a complete widget 'tree'
 *
 * @param widget Pointer to the VisUIWidget that needs to be destroyed.
 *
 * @return VISUAL_OK on succes, or errors that can be thrown by any of the destroyer functions.
 */
int visual_ui_widget_destroy (VisUIWidget *widget)
{
	visual_log_return_val_if_fail (widget != NULL, -VISUAL_ERROR_UI_WIDGET_NULL);

	return widget_destroy (widget);
}

/**
 * Sets a request for size to a VisUIWidget, to be used to request a certain dimension.
 *
 * @param widget Pointer to the VisUIWidget to which the size request is set.
 * @param width The width in pixels of the size requested.
 * @param height The height in pixels of the size requested.
 *
 * @return VISUAL_OK on succes, or -VISUAL_ERROR_UI_WIDGET_NULL on failure.
 */
int visual_ui_widget_set_size_request (VisUIWidget *widget, int width, int height)
{
	visual_log_return_val_if_fail (widget != NULL, -VISUAL_ERROR_UI_WIDGET_NULL);

	widget->width = width;
	widget->height = height;

	return VISUAL_OK;
}

/**
 * Sets a tooltip to a VisUIWidget.
 *
 * @param widget Pointer to the VisUIWidget to which the tooltip is set.
 * @param tooltip A string containing the tooltip text.
 *
 * @return VISUAL_OK on succes, or -VISUAL_ERROR_UI_WIDGET_NULL on failure.
 */
int visual_ui_widget_set_tooltip (VisUIWidget *widget, const char *tooltip)
{
	visual_log_return_val_if_fail (widget != NULL, -VISUAL_ERROR_UI_WIDGET_NULL);

	widget->tooltip = tooltip;

	return VISUAL_OK;
}

/**
 * Gets the top VisUIWidget from a VisUIWidget, this means that it will retrieve
 * the widget that is the parent of all underlaying widgets.
 *
 * @param widget Pointer to the VisUIWidget of which we want to have the top VisUIWidget.
 *
 * @return Pointer to the top VisUIWidget, or NULL on failure.
 */
VisUIWidget *visual_ui_widget_get_top (VisUIWidget *widget)
{
	VisUIWidget *above;
	VisUIWidget *prev = widget;

	visual_log_return_val_if_fail (widget != NULL, NULL);

	while ((above = visual_ui_widget_get_parent (widget)) != NULL) {
		prev = widget;
	}

	return prev;
}

/**
 * Gets the parent VisUIWidget from a VisUIWidget, this retrieves the parent of the given
 * widget.
 *
 * @param widget Pointer to the VisUIWidget of which the parent is requested.
 *
 * @return Pointer to the parent VisUIWidget, or NULL on failure.
 */
VisUIWidget *visual_ui_widget_get_parent (VisUIWidget *widget)
{
	visual_log_return_val_if_fail (widget != NULL, NULL);

	return widget->parent;
}

/**
 * Gets the VisUIWidgetType type from a VisUIWidget, this contains what kind of widget the given
 * VisUIWidget is.
 *
 * @param widget Pointer to the VisUIWidget of which the type is requested.
 *
 * @return The VisUIWidgetType of the given VisUIWidget.
 */
VisUIWidgetType visual_ui_widget_get_type (VisUIWidget *widget)
{
	visual_log_return_val_if_fail (widget != NULL, VISUAL_WIDGET_TYPE_NULL);

	return widget->type;
}

/**
 * Gets the private data from a VisUIWidget, this may be used by native UI drivers, and should never
 * be touched from within apps or plugins, unless you REALLY know what you're doing.
 *
 * @param widget Pointer to the VisUIWidget of which we want to retrieve it's private data.
 *
 * @return Pointer to the private data.
 */
void *visual_ui_widget_get_private (VisUIWidget *widget)
{
	visual_log_return_val_if_fail (widget != NULL, NULL);

	return widget->priv;
}

/**
 * Sets the private data for a VisUIWidget, this may be used by native UI drivers, and should never
 * be touched from within apps or plugins, unless you REALLY know what you're doing.
 *
 * @param widget Pointer to the VisUIWidget to which private data is set.
 * @param priv Pointer to the private data that is set.
 *
 * @return VISUAL_OK on succes, or -VISUAL_ERROR_UI_WIDGET_NULL on failure.
 */
int visual_ui_widget_set_private (VisUIWidget *widget, void *priv)
{
	visual_log_return_val_if_fail (widget != NULL, -VISUAL_ERROR_UI_WIDGET_NULL);

	widget->priv = priv;

	return VISUAL_OK;
}

/**
 * Adds a VisUIWidget to a VisUIContainer.
 *
 * @see visual_ui_box_pack
 * @see visual_ui_table_attach
 * 
 * @param container Pointer to the VisUIContainer in which a VisUIWidget is put.
 * @param widget Pointer to the VisUIWidget that is been put in the VisUIContainer.
 *
 * @return VISUAL_OK on succes, or -VISUAL_ERROR_UI_CONTAINER_NULL, -VISUAL_ERROR_UI_WIDGET_NULL on failure.
 */
int visual_ui_container_add (VisUIContainer *container, VisUIWidget *widget)
{
	visual_log_return_val_if_fail (container != NULL, -VISUAL_ERROR_UI_CONTAINER_NULL);
	visual_log_return_val_if_fail (widget != NULL, -VISUAL_ERROR_UI_WIDGET_NULL);

	container->child = widget;

	return VISUAL_OK;
}

/**
 * Gets the child VisUIWidget from a VisUIContainer.
 *
 * @param container Pointer to the VisUIContainer of which we want the child VisUIWidget.
 *
 * @return The child VisUIWidget, or NULL on failure.
 */
VisUIWidget *visual_ui_container_get_child (VisUIContainer *container)
{
	visual_log_return_val_if_fail (container != NULL, NULL);

	return container->child;
}

/**
 * Creates a new VisUIBox, that can be used to pack VisUIWidgets in.
 *
 * @param orient Indicates the orientation style of the box, being either
 * 	VISUAL_ORIENT_TYPE_HORIZONTAL or VISUAL_ORIENT_TYPE_VERTICAL.
 *
 * @return The newly created VisUIBox in the form of a VisUIWidget.
 */
VisUIWidget *visual_ui_box_new (VisUIOrientType orient)
{
	VisUIBox *box;

	box = visual_mem_new0 (VisUIBox, 1);
	VISUAL_UI_WIDGET (box)->type = VISUAL_WIDGET_TYPE_BOX;

	box->orient = orient;

	visual_ui_widget_set_size_request (VISUAL_UI_WIDGET (box), -1, -1);

	return VISUAL_UI_WIDGET (box);
}

/**
 * Frees a VisUIBox, this does not destroy it's childeren.
 *
 * @param box Pointer to the VisUIBox that needs to be freed.
 *
 * @return VISUAL_OK on succes, -VISUAL_ERROR_UI_BOX_NULL, -VISUAL_ERROR_UI_NO_BOX or
 * 	error values returned by visual_mem_free () on failure.
 */
int visual_ui_box_free (VisUIBox *box)
{
	visual_log_return_val_if_fail (box != NULL, -VISUAL_ERROR_UI_BOX_NULL);

	if (VISUAL_UI_WIDGET (box)->type != VISUAL_WIDGET_TYPE_BOX) {
		visual_log (VISUAL_LOG_CRITICAL, "Wrong free for VisUI widget");

		return -VISUAL_ERROR_UI_NO_BOX;
	}

	/* Delete the list entries, don't destroy the content */
	visual_list_destroy_elements (&box->childs, NULL);

	return visual_mem_free (box);
}

/**
 * Destroys a VisUIBox, this does destroy the VisUIBox and all it's childeren.
 *
 * @param box Pointer to the VisUIBox that needs to be destroyed.
 *
 * @return VISUAL_OK on succes, -VISUAL_ERROR_UI_BOX_NULL, -VISUAL_ERROR_UI_NO_BOX or
 * 	error values returned by visual_ui_box_free () on failure.
 */
int visual_ui_box_destroy (VisUIBox *box)
{
	visual_log_return_val_if_fail (box != NULL, -VISUAL_ERROR_UI_BOX_NULL);

	if (VISUAL_UI_WIDGET (box)->type != VISUAL_WIDGET_TYPE_BOX) {
		visual_log (VISUAL_LOG_CRITICAL, "Wrong destroyer for VisUI widget");

		return -VISUAL_ERROR_UI_NO_BOX;
	}

	visual_list_destroy_elements (&box->childs, list_widget_destroy);

	return visual_ui_box_free (box);
}

/**
 * Packs VisUIWidgets into a VisUIBox, this can be used to pack widgets either vertically or horizontally,
 * depending on the type of box.
 *
 * @param box Pointer to the VisUIBox in which the widget is packed.
 * @param widget Pointer to the VisUIWidget which is packed in the box.
 *
 * @return VISUAL_OK on succes, -VISUAL_ERROR_UI_BOX_NULL, -VISUAL_ERROR_UI_WIDGET_NULL or
 * 	error values returned by visual_list_add () on failure.
 */
int visual_ui_box_pack (VisUIBox *box, VisUIWidget *widget)
{
	visual_log_return_val_if_fail (box != NULL, -VISUAL_ERROR_UI_BOX_NULL);
	visual_log_return_val_if_fail (widget != NULL, -VISUAL_ERROR_UI_WIDGET_NULL);

	return visual_list_add (&box->childs, widget);
}

/**
 * Retrieve a VisList of VisUIWidget elements, being the childs of the VisUIBox.
 * 
 * @param box Pointer to the VisUIBox from which the childs are requested.
 * 
 * @return VisList containing the childs of the VisUIBox or NULL on error.
 */
VisList *visual_ui_box_get_childs (VisUIBox *box)
{
	VisUIWidget *next;
	VisListEntry *le = NULL;

	visual_log_return_val_if_fail (box != NULL, NULL);

	return &box->childs;
}

/**
 * Get the VisUIOrientType value from a VisUIBox.
 *
 * @param box VisUIBox from which the VisUIOrientType is requested.
 *
 * @return VisUIOrientType containing the orientation style for this VisUIBox.
 */
VisUIOrientType visual_ui_box_get_orient (VisUIBox *box)
{
	visual_log_return_val_if_fail (box != NULL, VISUAL_ORIENT_TYPE_NONE);

	return box->orient;
}

/**
 * Creates a new VisUITable, that can be used to attach VisUIWidgets to cells in the table.
 *
 * @param rows The number of rows in the table.
 * @param cols The number of columns in the table.
 *
 * @return The newly created VisUITable in the form of a VisUIWidget.
 */
VisUIWidget *visual_ui_table_new (int rows, int cols)
{
	VisUITable *table;

	table = visual_mem_new0 (VisUITable, 1);
	VISUAL_UI_WIDGET (table)->type = VISUAL_WIDGET_TYPE_TABLE;

	table->rows = rows;
	table->cols = cols;

	visual_ui_widget_set_size_request (VISUAL_UI_WIDGET (table), -1, -1);

	return VISUAL_UI_WIDGET (table);
}

/**
 * Frees a VisUITable, this does not destroy it's childeren.
 *
 * @param table Pointer to the VisUITable that needs to be freed.
 *
 * @return VISUAL_OK on succes, -VISUAL_ERROR_UI_TABLE_NULL, -VISUAL_ERROR_UI_NO_TABLE or 
 * 	error values returned by visual_mem_free () on failure.
 */
int visual_ui_table_free (VisUITable *table)
{
	visual_log_return_val_if_fail (table != NULL, -VISUAL_ERROR_UI_TABLE_NULL);

	if (VISUAL_UI_WIDGET (table)->type != VISUAL_WIDGET_TYPE_TABLE) {
		visual_log (VISUAL_LOG_CRITICAL, "Wrong free for VisUI widget");

		return -VISUAL_ERROR_UI_NO_TABLE;
	}

	/* Delete the list entries, don't destroy the widgets */
	visual_list_destroy_elements (&table->childs, free);

	return visual_mem_free (table);
}

/**
 * Destroys a VisUITable, this does destroy the VisUITable and all it's childeren.
 * 
 * @param table Pointer to the VisUITable that needs to be destroyed.
 *
 * @return VISUAL_OK on succes, -VISUAL_ERROR_UI_TABLE_NULL, -VISUAL_ERROR_UI_NO_TABLE or
 *	error values returned by visual_ui_table_free () on failure. 
 */
int visual_ui_table_destroy (VisUITable *table)
{
	visual_log_return_val_if_fail (table != NULL, -VISUAL_ERROR_UI_TABLE_NULL);

	if (VISUAL_UI_WIDGET (table)->type != VISUAL_WIDGET_TYPE_TABLE) {
		visual_log (VISUAL_LOG_CRITICAL, "Wrong destroyer for VisUI widget");

		return -VISUAL_ERROR_UI_NO_TABLE;
	}

	/* Destroy the widgets here, and the list in the table_free */
	visual_list_destroy_elements (&table->childs, table_entry_destroyer);

	return visual_ui_table_free (table);
}

/**
 * Attaches a VisUIWidget to a cell within a VisUITable.
 * 
 * @param table Pointer to the VisUITable to which a VisUiWidget is attached.
 * @param widget Pointer to the VisUIWidget that is being attached to the VisUITable.
 * @param row The row number starting at 0.
 * @param col The column number starting at 0.
 * 
 * @return VISUAL_OK on succes, -VISUAL_ERROR_UI_TABLE_NULL, -VISUAL_ERROR_UI_WIDGET_NULL or
 * 	error values returned by visual_list_add () on failure.
 */
int visual_ui_table_attach (VisUITable *table, VisUIWidget *widget, int row, int col)
{
	VisUITableEntry *tentry;

	visual_log_return_val_if_fail (table != NULL, -VISUAL_ERROR_UI_TABLE_NULL);
	visual_log_return_val_if_fail (widget != NULL, -VISUAL_ERROR_UI_WIDGET_NULL);

	tentry = visual_mem_new0 (VisUITableEntry, 1);

	tentry->row = row;
	tentry->col = col;

	tentry->widget = widget;

	return visual_list_add (&table->childs, tentry);
}

/**
 * Retrieve a VisList containing VisUITableEntry elements, in which the child VisUIWidget and it's place
 * 	in the VisUITable is stored.
 *
 * @param table Pointer to the VisUITable from which the childs are requested.
 *
 * @return VisList containing the childs of the VisUITable, or NULL on error.
 */
VisList *visual_ui_table_get_childs (VisUITable *table)
{
	visual_log_return_val_if_fail (table != NULL, NULL);

	return &table->childs;
}

/**
 * Creates a new VisUIFrame, which can be used to put a frame around a VisUIWidget.
 * 
 * @param name The name of this frame.
 *
 * @return The newly created VisUIFrame in the form of a VisUIWidget.
 */
VisUIWidget *visual_ui_frame_new (const char *name)
{
	VisUIFrame *frame;

	frame = visual_mem_new0 (VisUIFrame, 1);
	VISUAL_UI_WIDGET (frame)->type = VISUAL_WIDGET_TYPE_FRAME;

	frame->name = name;

	visual_ui_widget_set_size_request (VISUAL_UI_WIDGET (frame), -1, -1);
	
	return VISUAL_UI_WIDGET (frame);
}

/**
 * Frees a VisUIFrame, this does not destroy it's childeren.
 * 
 * @param frame Pointer to the VisUIFrame that needs to be freed.
 *
 * @return VISUAL_OK on succes, -VISUAL_ERROR_UI_FRAME_NULL, -VISUAL_ERROR_UI_NO_FRAME or
 *	error values returned by visual_mem_free () on failure.
 */
int visual_ui_frame_free (VisUIFrame *frame)
{
	visual_log_return_val_if_fail (frame != NULL, -VISUAL_ERROR_UI_FRAME_NULL);

	if (VISUAL_UI_WIDGET (frame)->type != VISUAL_WIDGET_TYPE_FRAME) {
		visual_log (VISUAL_LOG_CRITICAL, "Wrong free for VisUI widget");

		return -VISUAL_ERROR_UI_NO_FRAME;
	}

	return visual_mem_free (frame);
}

/**
 * Destroy a VisUIFrame, this does destroy the VisUIFrame and all it's childeren.
 *
 * @param frame Pointer to the VisUIFrame that needs to be destroyed.
 *
 * @return VISUAL_OK on succes, -VISUAL_ERROR_UI_FRAME_NULL, -VISUAL_ERROR_UI_NO_FRAME or
 * 	error values returned by visual_ui_frame_free () on failure.
 */
int visual_ui_frame_destroy (VisUIFrame *frame)
{
	visual_log_return_val_if_fail (frame != NULL, -VISUAL_ERROR_UI_FRAME_NULL);

	if (VISUAL_UI_WIDGET (frame)->type != VISUAL_WIDGET_TYPE_FRAME) {
		visual_log (VISUAL_LOG_CRITICAL, "Wrong destroyer for VisUI widget");

		return -VISUAL_ERROR_UI_NO_FRAME;
	}
	
	widget_destroy (VISUAL_UI_CONTAINER (frame)->child);

	return visual_ui_frame_free (frame);
}

/**
 * Creates a new VisUILabel, which can be used as a one line piece of text in an user interface.
 *
 * @param text Text of which the label consists.
 * @param bold Flag that indicates if a label should be drawn bold or not.
 *
 * @return The newly created VisUILabel in the form of a VisUIWidget.
 */
VisUIWidget *visual_ui_label_new (const char *text, int bold)
{
	VisUILabel *label;

	label = visual_mem_new0 (VisUILabel, 1);
	VISUAL_UI_WIDGET (label)->type = VISUAL_WIDGET_TYPE_LABEL;

	label->text = text;
	label->bold = bold;

	visual_ui_widget_set_size_request (VISUAL_UI_WIDGET (label), -1, -1);
	
	return VISUAL_UI_WIDGET (label);
}

/**
 * Frees a VisUILabel.
 * 
 * @param label Pointer to the VisUILabel that needs to be freed.
 *
 * @return VISUAL_OK on succes, -VISUAL_ERROR_UI_LABEL_NULL, -VISUAL_ERROR_UI_NO_LABEL or
 * 	error values returned by visual_mem_free () on failure.
 */
int visual_ui_label_free (VisUILabel *label)
{
	visual_log_return_val_if_fail (label != NULL, -VISUAL_ERROR_UI_LABEL_NULL);

	if (VISUAL_UI_WIDGET (label)->type != VISUAL_WIDGET_TYPE_LABEL) {
		visual_log (VISUAL_LOG_CRITICAL, "Wrong free for VisUI widget");

		return -VISUAL_ERROR_UI_NO_LABEL;
	}

	return visual_mem_free (label);
}

/**
 * Sets the bold flag for a VisUILabel.
 * 
 * @param label Pointer to the VisUILabel of which the bold flag is set, or unset.
 * @param bold Flag that indicates if a label should be drawn bold or not.
 *
 * @return VISUAL_OK on succes, -VISUAL_ERROR_UI_LABEL_NULL on failure.
 */
int visual_ui_label_set_bold (VisUILabel *label, int bold)
{
	visual_log_return_val_if_fail (label != NULL, -VISUAL_ERROR_UI_LABEL_NULL);

	label->bold = bold;

	return VISUAL_OK;
}

/**
 * Sets the text for a VisUILabel.
 *
 * @param label Pointer to the VisUILabel to which text is being set.
 * @param text The text that is being set to the VisUILabel.
 *
 * @return VISUAL_OK on succes, -VISUAL_ERROR_UI_LABEL_NULL on failure.
 */
int visual_ui_label_set_text (VisUILabel *label, const char *text)
{
	visual_log_return_val_if_fail (label != NULL, -VISUAL_ERROR_UI_LABEL_NULL);

	label->text = text;

	return VISUAL_OK;
}

/**
 * Retrieve the text from a VisUILabel.
 *
 * @param label Pointer to the VisUILabel from which the text is being requested.
 * 
 * return The text contained in the label, NULL on failure.
 */
const char *visual_ui_label_get_text (VisUILabel *label)
{
	visual_log_return_val_if_fail (label != NULL, NULL);

	return label->text;
}

/**
 * Creates a new VisUIImage, which can contain an image, loaded from a VisVideo.
 *
 * @param video The VisVideo containing the image to be displayed.
 *
 * @return The newly created VisUIImage in the form of a VisUIWidget.
 */
VisUIWidget *visual_ui_image_new (const VisVideo *video)
{
	VisUIImage *image;

	image = visual_mem_new0 (VisUIImage, 1);
	VISUAL_UI_WIDGET (image)->type = VISUAL_WIDGET_TYPE_IMAGE;

	image->image = video;

	visual_ui_widget_set_size_request (VISUAL_UI_WIDGET (image), -1, -1);

	return VISUAL_UI_WIDGET (image);
}

/**
 * Frees a VisUIImage. Keep in mind that this won't free the VisVideo that is connected to
 * the VisUIImage.
 * 
 * @param image Pointer to the VisUIImage that needs to be freed.
 *
 * @return VISUAL_OK on succes, -VISUAL_ERROR_UI_IMAGE_NULL, -VISUAL_ERROR_UI_NO_IMAGE or
 * 	error values returned by visual_mem_free () on failure.
 */
int visual_ui_image_free (VisUIImage *image)
{
	visual_log_return_val_if_fail (image != NULL, -VISUAL_ERROR_UI_IMAGE_NULL);

	if (VISUAL_UI_WIDGET (image)->type != VISUAL_WIDGET_TYPE_IMAGE) {
		visual_log (VISUAL_LOG_CRITICAL, "Wrong free for VisUI widget");

		return -VISUAL_ERROR_UI_NO_IMAGE;
	}

	return visual_mem_free (image);
}

/**
 * Sets a VisVideo to a VisUIImage. The VisVideo contains the content of the image.
 *
 * @param image Pointer to the VisUIImage to which the VisVideo is set.
 * @param video Pointer to the VisVideo that is set to the VisUIImage.
 *
 * @return VISUAL_OK on succes, -VISUAL_ERROR_UI_IMAGE_NULL on failure.
 */
int visual_ui_image_set_video (VisUIImage *image, const VisVideo *video)
{
	visual_log_return_val_if_fail (image != NULL, -VISUAL_ERROR_UI_IMAGE_NULL);

	image->image = video;

	return VISUAL_OK;
}

/**
 * Retrieves the VisVideo from a VisUIImage.
 *
 * @param image Pointer to the VisUIImage from which the VisVideo is requested.
 * 
 * return The VisVideo that is connected to the VisUIImage.
 */
const VisVideo *visual_ui_image_get_video (VisUIImage *image)
{
	visual_log_return_val_if_fail (image != NULL, NULL);

	return image->image;
}

/**
 * Creates a new VisUISeparator, which can function as a separation item between other VisUIWidgets.
 *
 * @param orient Indicates the orientation style of the separator, being either
 *	VISUAL_ORIENT_TYPE_HORIZONTAL or VISUAL_ORIENT_TYPE_VERTICAL.
 *
 * @return The newly created VisUISeparator in the form of a VisUIWidget.
 */
VisUIWidget *visual_ui_separator_new (VisUIOrientType orient)
{
	VisUISeparator *separator;

	separator = visual_mem_new0 (VisUISeparator, 1);
	VISUAL_UI_WIDGET (separator)->type = VISUAL_WIDGET_TYPE_SEPARATOR;

	separator->orient = orient;

	visual_ui_widget_set_size_request (VISUAL_UI_WIDGET (separator), -1, -1);

	return VISUAL_UI_WIDGET (separator);
}

/**
 * Frees a VisUISeperator.
 * 
 * @param seperatpr Pointer to the VisUISeparator that needs to be freed.
 *
 * @return VISUAL_OK on succes, -VISUAL_ERROR_UI_SEPARATOR_NULL, -VISUAL_ERROR_UI_NO_SEPARATOR or
 * 	error values returned by visual_mem_free () on failure.
 */
int visual_ui_separator_free (VisUISeparator *separator)
{
	visual_log_return_val_if_fail (separator != NULL, -VISUAL_ERROR_UI_SEPARATOR_NULL);

	if (VISUAL_UI_WIDGET (separator)->type != VISUAL_WIDGET_TYPE_SEPARATOR) {
		visual_log (VISUAL_LOG_CRITICAL, "Wrong free for VisUI widget");

		return -VISUAL_ERROR_UI_NO_SEPARATOR;
	}

	return visual_mem_free (separator);
}

/**
 * Get the VisUIOrientType value from a VisUISeparator.
 *
 * @param separator VisUISeparator from which the VisUIOrientType is requested.
 *
 * @return VisUIOrientType containing the orientation style for this VisUISeparator.
 */
VisUIOrientType visual_ui_separator_get_orient (VisUISeparator *separator)
{
	visual_log_return_val_if_fail (separator != NULL, VISUAL_ORIENT_TYPE_NONE);

	return separator->orient;
}

/* FIXME: doxygen link to the VisUI types enums and documentate the cmoplete class hierachy there */
/**
 * Links a VisParamEntry to a VisUIMutator type.
 *
 * @param mutator Pointer to the VisUIMutator to which the VisParamEntry is linked.
 * @param param Pointer to the VisParamEntry that is linked to the VisUIMutator.
 *
 * @return VISUAL_OK on succes, -VISUAL_ERROR_UI_MUTATOR_NULL or -VISUAL_ERROR_PARAM_NULL on failure.
 */
int visual_ui_mutator_set_param (VisUIMutator *mutator, const VisParamEntry *param)
{
	visual_log_return_val_if_fail (mutator != NULL, -VISUAL_ERROR_UI_MUTATOR_NULL);
	visual_log_return_val_if_fail (param != NULL, -VISUAL_ERROR_PARAM_NULL);

	/* FIXME Check if param is valid with mutator type, if not, give a critical */

	mutator->param = param;

	return VISUAL_OK;
}

const VisParamEntry *visual_ui_mutator_get_param (VisUIMutator *mutator)
{
	visual_log_return_val_if_fail (mutator != NULL, NULL);

	return mutator->param;
}

int visual_ui_range_set_properties (VisUIRange *range, double min, double max, double step, int precision)
{
	visual_log_return_val_if_fail (range != NULL, -VISUAL_ERROR_UI_RANGE_NULL);

	range->min = min;
	range->max = max;
	range->step = step;
	range->precision = precision;

	return VISUAL_OK;
}

int visual_ui_range_set_max (VisUIRange *range, double max)
{
	visual_log_return_val_if_fail (range != NULL, -VISUAL_ERROR_UI_RANGE_NULL);

	range->max = max;

	return VISUAL_OK;
}

int visual_ui_range_set_min (VisUIRange *range, double min)
{
	visual_log_return_val_if_fail (range != NULL, -VISUAL_ERROR_UI_RANGE_NULL);

	range->min = min;

	return VISUAL_OK;
}

int visual_ui_range_set_step (VisUIRange *range, double step)
{
	visual_log_return_val_if_fail (range != NULL, -VISUAL_ERROR_UI_RANGE_NULL);

	range->step = step;

	return VISUAL_OK;
}

int visual_ui_range_set_precision (VisUIRange *range, int precision)
{
	visual_log_return_val_if_fail (range != NULL, -VISUAL_ERROR_UI_RANGE_NULL);

	range->precision = precision;

	return VISUAL_OK;
}

VisUIWidget *visual_ui_entry_new ()
{
	VisUIEntry *entry;

	entry = visual_mem_new0 (VisUIEntry, 1);
	VISUAL_UI_WIDGET (entry)->type = VISUAL_WIDGET_TYPE_ENTRY;

	visual_ui_widget_set_size_request (VISUAL_UI_WIDGET (entry), -1, -1);

	return VISUAL_UI_WIDGET (entry);
}

int visual_ui_entry_free (VisUIEntry *entry)
{
	visual_log_return_val_if_fail (entry != NULL, -VISUAL_ERROR_UI_ENTRY_NULL);

	if (VISUAL_UI_WIDGET (entry)->type != VISUAL_WIDGET_TYPE_ENTRY) {
		visual_log (VISUAL_LOG_CRITICAL, "Wrong free for VisUI widget");

		return -VISUAL_ERROR_UI_NO_ENTRY;
	}

	return visual_mem_free (entry);
}

int visual_ui_entry_set_length (VisUIEntry *entry, int length)
{
	visual_log_return_val_if_fail (entry != NULL, -VISUAL_ERROR_UI_ENTRY_NULL);

	entry->length = length;

	return VISUAL_OK;
}

VisUIWidget *visual_ui_slider_new (int showvalue)
{
	VisUISlider *slider;

	slider = visual_mem_new0 (VisUISlider, 1);
	VISUAL_UI_WIDGET (slider)->type = VISUAL_WIDGET_TYPE_SLIDER;

	slider->showvalue = showvalue;

	visual_ui_widget_set_size_request (VISUAL_UI_WIDGET (slider), -1, -1);

	return VISUAL_UI_WIDGET (slider);
}

int visual_ui_slider_free (VisUISlider *slider)
{
	visual_log_return_val_if_fail (slider != NULL, -VISUAL_ERROR_UI_SLIDER_NULL);

	if (VISUAL_UI_WIDGET (slider)->type != VISUAL_WIDGET_TYPE_SLIDER) {
		visual_log (VISUAL_LOG_CRITICAL, "Wrong free for VisUI widget");

		return -VISUAL_ERROR_UI_NO_SLIDER;
	}

	return visual_mem_free (slider);
}

VisUIWidget *visual_ui_numeric_new ()
{
	VisUINumeric *numeric;

	numeric = visual_mem_new0 (VisUINumeric, 1);
	VISUAL_UI_WIDGET (numeric)->type = VISUAL_WIDGET_TYPE_NUMERIC;

	visual_ui_widget_set_size_request (VISUAL_UI_WIDGET (numeric), -1, -1);

	return VISUAL_UI_WIDGET (numeric);
}

int visual_ui_numeric_free (VisUINumeric *numeric)
{
	visual_log_return_val_if_fail (numeric != NULL, -VISUAL_ERROR_UI_NUMERIC_NULL);

	if (VISUAL_UI_WIDGET (numeric)->type != VISUAL_WIDGET_TYPE_NUMERIC) {
		visual_log (VISUAL_LOG_CRITICAL, "Wrong free for VisUI widget");

		return -VISUAL_ERROR_UI_NO_NUMERIC;
	}

	return visual_mem_free (numeric);
}

VisUIWidget *visual_ui_color_new ()
{
	VisUIColor *color;

	color = visual_mem_new0 (VisUIColor, 1);
	VISUAL_UI_WIDGET (color)->type = VISUAL_WIDGET_TYPE_COLOR;

	visual_ui_widget_set_size_request (VISUAL_UI_WIDGET (color), -1, -1);

	return VISUAL_UI_WIDGET (color);
}

int visual_ui_color_free (VisUIColor *color)
{
	visual_log_return_val_if_fail (color != NULL, -VISUAL_ERROR_UI_COLOR_NULL);

	if (VISUAL_UI_WIDGET (color)->type != VISUAL_WIDGET_TYPE_COLOR) {
		visual_log (VISUAL_LOG_CRITICAL, "Wrong free for VisUI widget");

		return -VISUAL_ERROR_UI_NO_COLOR;
	}

	return visual_mem_free (color);
}

int visual_ui_choice_add (VisUIChoice *choice, const char *name, const VisParamEntry *value)
{
	VisUIChoiceEntry *centry;

	visual_log_return_val_if_fail (choice != NULL, -VISUAL_ERROR_UI_CHOICE_NULL);
	visual_log_return_val_if_fail (name != NULL, -VISUAL_ERROR_NULL);
	visual_log_return_val_if_fail (value != NULL, -VISUAL_ERROR_PARAM_NULL);

	centry = visual_mem_new0 (VisUIChoiceEntry, 1);

	centry->name = name;
	centry->value = value;

	choice->choices.count++;
	/* FIXME be aware on object destroy, that this needs to be destroyed as well, watch out!! */
	visual_list_add (&choice->choices.choices, centry);

	return VISUAL_OK;
}

int visual_ui_choice_add_many (VisUIChoice *choice, const VisParamEntry *paramchoices)
{
	VisUIChoiceEntry *centry;
	int i = 0;

	visual_log_return_val_if_fail (choice != NULL, -VISUAL_ERROR_UI_CHOICE_NULL);
	visual_log_return_val_if_fail (paramchoices != NULL, -VISUAL_ERROR_PARAM_NULL);

	while (paramchoices[i].type != VISUAL_PARAM_TYPE_END) {
		visual_ui_choice_add (choice, paramchoices[i].name, &paramchoices[i]);

		i++;
	}

	return VISUAL_OK;
}

int visual_ui_choice_free_choices (VisUIChoice *choice)
{
	visual_log_return_val_if_fail (choice != NULL, -VISUAL_ERROR_UI_CHOICE_NULL);

	return visual_list_destroy_elements (&choice->choices.choices, free); 
}

int visual_ui_choice_set_active (VisUIChoice *choice, int index)
{
	VisUIChoiceEntry *centry;
	const VisParamEntry *param;
	VisParamEntry *newparam;

	visual_log_return_val_if_fail (choice != NULL, -VISUAL_ERROR_UI_CHOICE_NULL);

	centry = visual_ui_choice_get_choice (choice, index);
	visual_log_return_val_if_fail (centry != NULL, -VISUAL_ERROR_UI_CHOICE_ENTRY_NULL);

	param = visual_ui_mutator_get_param (VISUAL_UI_MUTATOR (choice));

	newparam = (VisParamEntry *) centry->value;

	return visual_param_entry_set_from_param (param, newparam);
}

int visual_ui_choice_get_active (VisUIChoice *choice)
{
	VisListEntry *le = NULL;
	VisUIChoiceEntry *centry;
	VisParamEntry *param;
	int i = 0;

	visual_log_return_val_if_fail (choice != NULL, -VISUAL_ERROR_UI_CHOICE_NULL);

	param = visual_ui_mutator_get_param (VISUAL_UI_MUTATOR (choice));

	while ((centry = visual_list_next (&choice->choices.choices, &le)) != NULL) {
		const VisParamEntry *cparam;
		
		cparam = centry->value;

		if (visual_param_entry_compare (param, cparam) == TRUE)
			return i;

		i++;
	}

	return -VISUAL_ERROR_UI_CHOICE_NONE_ACTIVE;
}

VisUIChoiceEntry *visual_ui_choice_get_choice (VisUIChoice *choice, int index)
{
	visual_log_return_val_if_fail (choice != NULL, NULL);

	return visual_list_get (&choice->choices.choices, index);
}

VisUIChoiceList *visual_ui_choice_get_choices (VisUIChoice *choice)
{
	visual_log_return_val_if_fail (choice != NULL, NULL);

	return &choice->choices;
}

VisUIWidget *visual_ui_popup_new ()
{
	VisUIPopup *popup;

	popup = visual_mem_new0 (VisUIPopup, 1);
	VISUAL_UI_WIDGET (popup)->type = VISUAL_WIDGET_TYPE_POPUP;

	VISUAL_UI_CHOICE (popup)->choices.type = VISUAL_CHOICE_TYPE_SINGLE;

	visual_ui_widget_set_size_request (VISUAL_UI_WIDGET (popup), -1, -1);

	return VISUAL_UI_WIDGET (popup);
}

int visual_ui_popup_free (VisUIPopup *popup)
{
	visual_log_return_val_if_fail (popup != NULL, -VISUAL_ERROR_UI_POPUP_NULL);

	if (VISUAL_UI_WIDGET (popup)->type != VISUAL_WIDGET_TYPE_POPUP) {
		visual_log (VISUAL_LOG_CRITICAL, "Wrong free for VisUI widget");

		return -VISUAL_ERROR_UI_NO_POPUP;
	}

	visual_ui_choice_free_choices (VISUAL_UI_CHOICE (popup));

	return visual_mem_free (popup);
}

VisUIWidget *visual_ui_list_new ()
{
	VisUIList *list;

	list = visual_mem_new0 (VisUIList, 1);
	VISUAL_UI_WIDGET (list)->type = VISUAL_WIDGET_TYPE_LIST;

	VISUAL_UI_CHOICE (list)->choices.type = VISUAL_CHOICE_TYPE_SINGLE;

	visual_ui_widget_set_size_request (VISUAL_UI_WIDGET (list), -1, -1);

	return VISUAL_UI_WIDGET (list);
}

int visual_ui_list_free (VisUIList *list)
{
	visual_log_return_val_if_fail (list != NULL, -VISUAL_ERROR_UI_LIST_NULL);

	if (VISUAL_UI_WIDGET (list)->type != VISUAL_WIDGET_TYPE_LIST) {
		visual_log (VISUAL_LOG_CRITICAL, "Wrong free for VisUI widget");

		return -VISUAL_ERROR_UI_NO_LIST;
	}

	visual_ui_choice_free_choices (VISUAL_UI_CHOICE (list));

	return visual_mem_free (list);
}

VisUIWidget *visual_ui_radio_new (VisUIOrientType orient)
{
	VisUIRadio *radio;

	radio = visual_mem_new0 (VisUIRadio, 1);
	VISUAL_UI_WIDGET (radio)->type = VISUAL_WIDGET_TYPE_RADIO;

	VISUAL_UI_CHOICE (radio)->choices.type = VISUAL_CHOICE_TYPE_SINGLE;

	radio->orient = orient;
	
	visual_ui_widget_set_size_request (VISUAL_UI_WIDGET (radio), -1, -1);

	return VISUAL_UI_WIDGET (radio);
}

int visual_ui_radio_free (VisUIRadio *radio)
{
	visual_log_return_val_if_fail (radio != NULL, -VISUAL_ERROR_UI_RADIO_NULL);

	if (VISUAL_UI_WIDGET (radio)->type != VISUAL_WIDGET_TYPE_RADIO) {
		visual_log (VISUAL_LOG_CRITICAL, "Wrong free for VisUI widget");

		return -VISUAL_ERROR_UI_NO_RADIO;
	}

	visual_ui_choice_free_choices (VISUAL_UI_CHOICE (radio));

	return visual_mem_free (radio);
}

VisUIWidget *visual_ui_checkbox_new (const char *name, int boolcheck)
{
	VisUICheckbox *checkbox;
	static const VisParamEntry truefalse[] = {
		VISUAL_PARAM_LIST_ENTRY_INTEGER ("false",	FALSE),
		VISUAL_PARAM_LIST_ENTRY_INTEGER ("true",	TRUE),
		VISUAL_PARAM_LIST_END
	};

	checkbox = visual_mem_new0 (VisUICheckbox, 1);
	VISUAL_UI_WIDGET (checkbox)->type = VISUAL_WIDGET_TYPE_CHECKBOX;

	VISUAL_UI_CHOICE (checkbox)->choices.type = VISUAL_CHOICE_TYPE_SINGLE;

	checkbox->name = name;

	/* Boolean checkbox, generate a FALSE, TRUE choicelist ourself */
	if (boolcheck == TRUE)
		visual_ui_choice_add_many (VISUAL_UI_CHOICE (checkbox), truefalse);

	visual_ui_widget_set_size_request (VISUAL_UI_WIDGET (checkbox), -1, -1);

	return VISUAL_UI_WIDGET (checkbox);
}

int visual_ui_checkbox_free (VisUICheckbox *checkbox)
{
	visual_log_return_val_if_fail (checkbox != NULL, -VISUAL_ERROR_UI_CHECKBOX_NULL);

	if (VISUAL_UI_WIDGET (checkbox)->type != VISUAL_WIDGET_TYPE_CHECKBOX) {
		visual_log (VISUAL_LOG_CRITICAL, "Wrong free for VisUI widget");

		return -VISUAL_ERROR_UI_NO_CHECKBOX;
	}

	visual_ui_choice_free_choices (VISUAL_UI_CHOICE (checkbox));

	return visual_mem_free (checkbox);
}

/**
 * @}
 */
