long blockmma_send_task(struct blockmma_cmd __user *user_cmd);
int blockmma_sync(struct blockmma_cmd __user *user_cmd);
int blockmma_get_task(struct blockmma_hardware_cmd __user *user_cmd);
int blockmma_comp(struct blockmma_hardware_cmd __user *user_cmd);
int blockmma_author(struct blockmma_hardware_cmd __user *user_cmd);
