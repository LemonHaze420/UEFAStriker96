extern	Spring	strong_spring;
extern	Spring	weak_spring;


extern	void	(*physics[])(Object *obj );



extern	void matrix_translate( RT *vo, VECTOR *vi );
extern	void apply_force_ws( VECTOR *force, VECTOR *rel_position, Object *obj );
extern	void apply_force( VECTOR *force, VECTOR *rel_position, RT *m );
extern	void vector_matrix( VECTOR *t, RT *m1 );
extern	void svector_matrix( SVECTOR *sv, RT *om );
extern	void svector_X_matrix( SVECTOR *t, RT *m, VECTOR *ot );
extern	void vector_X_matrix( VECTOR *t, RT *m, VECTOR *ot );
extern	void rot_axis_set( SVECTOR *t, RT *m1 );
extern	void rv_axis_set( WORD	x, WORD y, WORD z, RT *m1 );
extern	void identity( RT *m );
extern	void matrix_rotate( RT *m, VECTOR *r );
extern	void identity( RT *m );

extern	void vector_shift( VECTOR *v, LONG shift, VECTOR *o );
extern	void vector_deshift( VECTOR *v, LONG shift, VECTOR *o );
extern	LONG vector_dot( VECTOR *a, VECTOR *b );
extern	LONG vector_magnitude( VECTOR *v );
extern	void vector_descale( VECTOR *v, LONG scale, VECTOR *o );
extern	void vector_cross( VECTOR *i, VECTOR *j, VECTOR *k  );
extern	void vector_scale( VECTOR *v, LONG scale, VECTOR *o );
extern	void vector_add( VECTOR *a, VECTOR *b, VECTOR *c );
extern	void vector_subtract( VECTOR *a, VECTOR *b, VECTOR *c );
extern	void vector_multiply( VECTOR *v, VECTOR *v1, VECTOR *o );
extern	void vector_negate( VECTOR *v, VECTOR *vo );

extern	void apply_impulse( VECTOR *force, VECTOR *rel_position, RT *m, Object *obj );
extern	void extract_x_component( Rotation *r, VECTOR *v );
extern	void extract_y_component( Rotation *r, VECTOR *v );
extern	void extract_z_component( Rotation *r, VECTOR *v );


extern	void event( UBYTE *string );
extern	void process_event( void );

