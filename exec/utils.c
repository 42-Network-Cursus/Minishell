void	unlink_error(void)
{
	error_mess("Minishell:", "Unlink failed", 0, 255);
	exit(0);
}