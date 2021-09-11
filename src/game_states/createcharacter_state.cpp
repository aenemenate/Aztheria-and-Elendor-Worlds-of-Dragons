#include "createcharacter_state.h"
#include "createworld_state.h"
#include "play_state.h"
#include "state_funcs.h"

#include "../base.h"
#include "../draw_funcs.h"
#include "../input_funcs.h"
#include "../ecs/entity.h"
#include "../world/world.h"
#include "../world_gen/entitygen_helper.h"

#include <algorithm>
#include <sstream>

int numAttributesSelected = 0;
int numMajorSkillsSelected = 0;
int numMinorSkillsSelected = 0;

CreateCharacterState CreateCharacterState::create_character_state;

class TextInputBox {
  int x, y;
public:
  std::string text;
  TextInputBox(int x, int y) : x(x), y(y) { text = ""; }
  void Update() {
    if (TerminalGetKey() == (MTK_MOUSE_LEFT|MTK_KEY_RELEASED) 
    && TerminalGetMouseX() >= x && TerminalGetMouseX() < x + 20 
    && TerminalGetMouseY() == y) {
      text = "";
      Draw();
      text = TerminalReadString(x, y);
    }
  }
  void Draw() {
    std::string spaces = "";
    for (int i = 0; i < 20; ++i)
      spaces += " ";
    PrintGraphic(x, y, {spaces, "white", "gray"});
    PrintGraphic(x, y, {text, "white", "gray"});
  }
};

TextInputBox nameBox = TextInputBox(30, 2);

// attributes

void SetAttribute(int attNum) {
  if (numAttributesSelected < 3 && std::string{CreateCharacterState::Instance()->buttons[attNum].GetBgColor()} != "gray") {
    ++numAttributesSelected;
    CreateCharacterState::Instance()->buttons[attNum].SetBgColor("gray");
  }
  else if (std::string{CreateCharacterState::Instance()->buttons[attNum].GetBgColor()} == "gray") {
    --numAttributesSelected;
    CreateCharacterState::Instance()->buttons[attNum].SetBgColor("black");
  }
}

void SetStrength(Game* game) {
  SetAttribute(0);
}

void SetIntelligence(Game* game) {
  SetAttribute(1);
}

void SetWillpower(Game* game) {
  SetAttribute(2);
}

void SetDexterity(Game* game) {
  SetAttribute(3);
}

void SetSpeed(Game* game) {
  SetAttribute(4);
}

void SetEndurance(Game* game) {
  SetAttribute(5);
}

void SetPersonality(Game* game) {
  SetAttribute(6);
}

void SetLuck(Game* game) {
  SetAttribute(7);
}

// skills

void SetSkill(int skillNum) {
  if (std::string{CreateCharacterState::Instance()->buttons[8 + skillNum].GetBgColor()} == "black") {
    if (numMajorSkillsSelected < 4) {
      ++numMajorSkillsSelected;
      CreateCharacterState::Instance()->buttons[8 + skillNum].SetBgColor("blue");
    }
    else if (numMinorSkillsSelected < 4) {
      ++numMinorSkillsSelected;
      CreateCharacterState::Instance()->buttons[8 + skillNum].SetBgColor("dark yellow");
    }
  }
  else if (std::string{CreateCharacterState::Instance()->buttons[8 + skillNum].GetBgColor()} == "blue" ) {
    if (numMinorSkillsSelected < 4) {
      ++numMinorSkillsSelected;
      --numMajorSkillsSelected;
      CreateCharacterState::Instance()->buttons[8 + skillNum].SetBgColor("dark yellow");
    }
    else {
      --numMajorSkillsSelected;
      CreateCharacterState::Instance()->buttons[8 + skillNum].SetBgColor("black");
    }
  }
  else if (std::string{CreateCharacterState::Instance()->buttons[8 + skillNum].GetBgColor()} == "dark yellow") {
    --numMinorSkillsSelected;
    CreateCharacterState::Instance()->buttons[8 + skillNum].SetBgColor("black");
  }
}
// strength
void SetForging(Game* game) { SetSkill(0); }

void SetHeavyWeapons(Game* game) { SetSkill(1); }

void SetLongBlades(Game* game) { SetSkill(2); }

void SetBlocking(Game* game) { SetSkill(3); }
// intelligence
void SetAlchemy(Game* game) { SetSkill(4); }

void SetConjuration(Game* game) { SetSkill(5); }

void SetEnchant(Game* game) { SetSkill(6); }

void SetLockpick(Game* game) { SetSkill(7); }
// willpower
void SetDestruction(Game* game) { SetSkill(8); }

void SetRestoration(Game* game) { SetSkill(9); }

void SetIllusion(Game* game) { SetSkill(10); }
// dexterity
void SetLightArmor(Game* game) { SetSkill(11); }

void SetMarksmanship(Game* game) { SetSkill(12); }

void SetSneaking(Game* game) { SetSkill(13); }

void SetCrafting(Game* game) { SetSkill(14); }
// speed
void SetAcrobatics(Game* game) { SetSkill(15); }

void SetBrawling(Game* game) { SetSkill(16); }

void SetShortBlade(Game* game) { SetSkill(17); }

void SetUnarmored(Game* game) { SetSkill(18); }
// endurance
void SetHeavyArmor(Game* game) { SetSkill(19); }

void SetSpear(Game* game) { SetSkill(20); }

void SetSwimming(Game* game) { SetSkill(21); }
// personality
void SetMercantile(Game* game) { SetSkill(22); }

void SetSpeech(Game* game) { SetSkill(23); }

void CreateCharacter(Game* game) {
  if (numAttributesSelected != 3 || numMajorSkillsSelected != 4 || numMinorSkillsSelected != 4 || nameBox.text == "")
    return;
  game->world = new World(1,1,1,1,1);
  Entity player = Entity(0);
  player.AddComponent(std::make_shared<Renderable>(Renderable({"@", "yellow", "black"})));
  player.AddComponent(std::make_shared<Name>(Name(std::string{nameBox.text})));
  player.AddComponent(std::make_shared<Fov>(Fov(28)));
  player.AddComponent(std::make_shared<Player>(Player(true)));
  player.AddComponent(std::make_shared<ActionTime>(ActionTime(Time())));
  player.AddComponent(std::make_shared<Inventory>(Inventory()));
  std::vector<BodyPart> bodyParts;
  bodyParts.push_back(BodyPart(PHand, "right hand"));
  bodyParts.push_back(BodyPart(PHand, "left hand"));
  bodyParts.push_back(BodyPart(PHead, "head"));
  bodyParts.push_back(BodyPart(PBody, "body"));
  bodyParts.push_back(BodyPart(PLegs, "legs"));
  bodyParts.push_back(BodyPart(PFeet, "feet"));
  player.AddComponent(std::make_shared<Equipment>(Equipment(bodyParts)));
  std::vector<Attribute> majorAttributes;
  for (int i = 0; i <= 7; ++i) {
    if (std::string{CreateCharacterState::Instance()->buttons[i].GetBgColor()} == "gray")
      majorAttributes.push_back((Attribute)i);
  }
  std::vector<Skill> majorSkills;
  for (int i = 0; i < 24; ++i) {
    if (std::string{CreateCharacterState::Instance()->buttons[8+i].GetBgColor()} == "blue")
      majorSkills.push_back((Skill)i);
  }
  std::vector<Skill> minorSkills;
  for (int i = 0; i < 24; ++i) {
    if (std::string{CreateCharacterState::Instance()->buttons[8+i].GetBgColor()} == "dark yellow")
      minorSkills.push_back((Skill)i);
  }
  std::shared_ptr<Class> uClass = std::make_shared<Class>(Class(majorAttributes, majorSkills, minorSkills));
  player.AddComponent(std::make_shared<Stats>(Stats(uClass)));
  player.AddComponent(uClass);
  game->world->AddEntity(player);
  EquipEntity(0, game->world);
  game->PushState(CreateWorldState::Instance());
  game->Update();
}

void CreateCharacterState::Init(Game* game) {
  numAttributesSelected = 0;
  numMajorSkillsSelected = 0;
  numMinorSkillsSelected = 0;
  buttons.clear();
  int term_width  = GetTermWidth(), 
      term_height = GetTermHeight();
// attribute buttons
  buttons.push_back(Button(1, 4, "Strength:", SetStrength));
  buttons.push_back(Button(1, 8, "Intelligence:", SetIntelligence));
  buttons.push_back(Button(1, 12, "Willpower:", SetWillpower));
  buttons.push_back(Button(1, 15, "Dexterity:", SetDexterity));
  buttons.push_back(Button(1, 19, "Speed:", SetSpeed));
  buttons.push_back(Button(1, 23, "Endurance:", SetEndurance));
  buttons.push_back(Button(1, 26, "Personality:", SetPersonality));
  buttons.push_back(Button(1, 28, "Luck:", SetLuck));
// skill buttons
// strength
  buttons.push_back(Button(15, 4, "Forging", SetForging));
  buttons.push_back(Button(15, 5, "HeavyWeapons", SetHeavyWeapons));
  buttons.push_back(Button(15, 6, "LongBlades", SetLongBlades));
  buttons.push_back(Button(15, 7, "Blocking", SetBlocking));
// intelligence
  buttons.push_back(Button(15, 8, "Alchemy", SetAlchemy));
  buttons.push_back(Button(15, 9, "Conjuration", SetConjuration));
  buttons.push_back(Button(15, 10, "Enchant", SetEnchant));
  buttons.push_back(Button(15, 11, "Lockpick", SetLockpick));
// willpower
  buttons.push_back(Button(15, 12, "Destruction", SetDestruction));
  buttons.push_back(Button(15, 13, "Restoration", SetRestoration));
  buttons.push_back(Button(15, 14, "Illusion", SetIllusion));
// dexterity
  buttons.push_back(Button(15, 15, "LightArmor", SetLightArmor));
  buttons.push_back(Button(15, 16, "Marksmanship", SetMarksmanship));
  buttons.push_back(Button(15, 17, "Sneaking", SetSneaking));
  buttons.push_back(Button(15, 18, "Crafting", SetCrafting));
// speed
  buttons.push_back(Button(15, 19, "Acrobatics", SetAcrobatics));
  buttons.push_back(Button(15, 20, "Brawling", SetBrawling));
  buttons.push_back(Button(15, 21, "ShortBlade", SetShortBlade));
  buttons.push_back(Button(15, 22, "Unarmored", SetUnarmored));
// endurance
  buttons.push_back(Button(15, 23, "HeavyArmor", SetHeavyArmor));
  buttons.push_back(Button(15, 24, "Spear", SetSpear));
  buttons.push_back(Button(15, 25, "Swimming", SetSwimming));
// personality
  buttons.push_back(Button(15, 26, "Mercantile", SetMercantile));
  buttons.push_back(Button(15, 27, "Speech", SetSpeech));
// create character button
  buttons.push_back(Button(12,term_height-2, "[[c]]reate character", CreateCharacter));
// go back button
  buttons.push_back(Button(1,term_height-2, "go [[b]]ack", GoBack));
}

void CreateCharacterState::Cleanup() {
  buttons.clear();
}

void CreateCharacterState::Resize(Game* game) {
  int term_width  = GetTermWidth(), 
      term_height = GetTermHeight();
// create character button
  CreateCharacterState::Instance()->buttons[CreateCharacterState::Instance()->buttons.size() - 2].SetY(term_height - 2);
  CreateCharacterState::Instance()->buttons[CreateCharacterState::Instance()->buttons.size() - 1].SetY(term_height - 2);
}

void CreateCharacterState::HandleEvents(Game* game) {
// reset our graphical elements if terminal is resized
  if (TerminalWasResized())
    Resize(game);
// handle key input
  switch (TerminalGetKey()) {
    case MTK_C:
      CreateCharacter(game);
      break;
    case MTK_ESCAPE:
    case MTK_B:
      GoBack(game);
      break;
  }
}

void CreateCharacterState::Update(Game* game) {
  nameBox.Update();
// update buttons
  for (int b=0;b<buttons.size();b++)
    buttons[b].Update(game);
}

void CreateCharacterState::Draw(Game* game) {
  int term_width  = GetTermWidth(), 
      term_height = GetTermHeight();
// draw buttons
  PrintGraphic(1, 1, {"Attributes:", "white", "black"});
  PrintGraphic(15, 1, {"Major", "blue", "black"});
  PrintGraphic(21, 1, {"&", "white", "black"});
  PrintGraphic(23, 1, {"Minor", "dark yellow", "black"});
  PrintGraphic(15, 2, {"Skills:", "white", "black"});
  PrintGraphic(30, 1, {"Name:", "white", "black"});
  nameBox.Draw();
  for (int b = 0; b < buttons.size(); ++b)
    buttons[b].Render();
}
