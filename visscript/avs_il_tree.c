#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "avs.h"


/**
 * Return tree base 
 *
 * @param ctx IL Tree context.
 */
ILInstruction * avs_il_tree_base(AvsILTreeContext *ctx)
{
	return ctx->base->insn.base;
}

void avs_il_tree_merge(AvsILTreeContext *ctx, AvsILTreeNode *node)
{
	AvsILTreeNode *current = ctx->current;

	if (!node->insn.base)
		return;

	if (!current->insn.end) {
		current->insn.base = node->insn.base;
		current->insn.end = node->insn.end;
	} else {
		node->insn.base->prev = current->insn.end;
		current->insn.end->next = node->insn.base;
		current->insn.end = node->insn.end;
	}
}

/**
 * Add a instruction to the IL Tree.
 *
 * @param ctx IL Tree context.
 * @param type Instruction type
 *
 * @return Newly created instruction on success, NULL on failure
 */
void avs_il_tree_add(AvsILTreeContext *ctx, ILInstruction *insn)
{
	AvsILTreeNode *node = ctx->current;
	
	if (!node->insn.end) {
		node->insn.base = insn;
		node->insn.end = insn;
	} else {
		insn->prev = node->insn.end;
		insn->next = NULL;

		node->insn.end->next = insn;
		node->insn.end = insn;
	}
}

/**
 * Reset an IL Tree context.
 *
 * @param ctx IL Tree context to reset.
 *
 * @return VISUAL_OK on success, VISUAL_ERROR_GENERAL on error.
 */
int avs_il_tree_reset(AvsILTreeContext *ctx)
{
	/* Reset instruction stack */
	avs_stack_reset(ctx->ixstack);

	/* Reset tree pointers */
	ctx->base = ctx->currentlevel = ctx->current = NULL;
	
	/* Allocate base tree node */
	avs_il_tree_node_init(ctx);

	return VISUAL_OK;
}

/**
 * Cleanup an IL Tree context.
 *
 * @param ctx IL Tree context to cleanup.
 *
 * @return VISUAL_OK on success, VISUAL_ERROR_GENERAL on error.
 */
int avs_il_tree_cleanup(AvsILTreeContext *ctx)
{
	/* Cleanup instruction stack */
	visual_object_unref(VISUAL_OBJECT(ctx->ixstack));

	return VISUAL_OK;
}

/* Initialize an IL tree context.
 *
 * @param ctx IL tree context to initialize.
 *
 * @return VISUAL_OK on success, VISUAL_ERROR_GENERAL on error.
 */
int avs_il_tree_init(AvsILTreeContext *ctx)
{
	memset(ctx, 0, sizeof(AvsILTreeContext));

	/* Allocate base tree node */
	avs_il_tree_node_init(ctx);

	/* Allocate Instruction Stack */
	ctx->ixstack = avs_stack_new0(ILInstruction, AVS_ILTREE_INITIAL_STACKSIZE,
						     AVS_ILTREE_MAXIMUM_STACKSIZE);

	return VISUAL_OK;
}

