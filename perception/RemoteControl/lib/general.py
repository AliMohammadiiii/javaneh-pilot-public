import time
def now():
    return time.time_ns() // (10**8)
def can_send_check(ms, last_can_time):
    if last_can_time == 0 or last_can_time < now() - ms:
        return True
    return False
