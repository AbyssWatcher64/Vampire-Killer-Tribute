#define OBJECT_HELMET	0x01
#define OBJECT_SWORD	0x02
#define OBJECT_KEY		0x04
#define OBJECT_WATER	0x08
#define OBJECT_BOOTS	0x10

class Player
{
public:
	void AddObject(int obj_id);
	bool HasObject(int obj_id) const;
private:
	int objects;
}

void Player::AddObject(int obj_id)
{
	objects = objects | obj_id;
}
bool Player::HasObject(int obj_id) const
{
	return objects & obj_id;
}

if(HasObject(OBJECT_BOOTS)) { ... }
AddObject(OBJECT_KEY);