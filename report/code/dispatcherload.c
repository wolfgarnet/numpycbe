spe_program_handle_t *dispatcher = spe_image_open( "dispatcher" );

// Load program into context
if ( spe_program_load ( spedata->spe_ctx, dispatcher ) )
{
     perror ( "Failed loading program" );
     return -1;
}
