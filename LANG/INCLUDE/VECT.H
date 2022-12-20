extern	void rv_axis_set_local( WORD	x, WORD y, WORD z, RT *m1 );
extern void vector_matrix( VECTOR *t, RT *om );
extern void svector_matrix( SVECTOR *sv, RT *om );
extern void svector_X_matrix( SVECTOR *t, RT *m, VECTOR *ot );
extern void vector_X_matrix( VECTOR *t, RT *m, VECTOR *ot );
extern void rot_axis_set( SVECTOR *t, RT *m1 );
extern void rv_axis_set( WORD	x, WORD y, WORD z, RT *m1 );
extern void matrix_translate( RT *vo, VECTOR *vi );
extern void matrix_rotate( RT *m, VECTOR *r );
extern void vector_cross( VECTOR *i, VECTOR *j, VECTOR *k  );
extern LONG vector_dot( VECTOR *a, VECTOR *b );
extern void vector_scale( VECTOR *v, LONG scale, VECTOR *o );
extern void vector_descale( VECTOR *v, LONG scale, VECTOR *o );
extern void vector_shift( VECTOR *v, LONG shift, VECTOR *o );
extern void vector_deshift( VECTOR *v, LONG shift, VECTOR *o );
extern void vector_add( VECTOR *a, VECTOR *b, VECTOR *c );
extern void vector_subtract( VECTOR *a, VECTOR *b, VECTOR *c );
extern LONG vector_magnitude( VECTOR *v );
extern LONG vector_size( VECTOR *v );
extern void vector_multiply( VECTOR *v, VECTOR *v1, VECTOR *o );
extern void vector_divide( VECTOR *v, VECTOR *v1, VECTOR *o );
extern void vector_negate( VECTOR *v, VECTOR *vo );
extern void identity( RT *m );
extern void inverse_matrix( RT *mat, RT *imat );
extern void extract_x_component( Rotation *r, VECTOR *v );
extern void extract_y_component( Rotation *r, VECTOR *v );
extern void extract_z_component( Rotation *r, VECTOR *v );

