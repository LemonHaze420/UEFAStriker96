extern	void unlink_group( Chain *chain, Link *first, Link *last );
extern	void *link( Chain *c );
extern	void unlink( void *object, Chain *c );
extern	void initialise_chain( LONG size, WORD many, Chain *chain );
extern	WORD	count_chain_free_links( Chain *chain );
extern	WORD	count_chain_active_links( Chain *chain );
extern	void reset_chain( Chain *chain );


extern	void link_group( Chain *chain, Link **first, Link **last, WORD many );
