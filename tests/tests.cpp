#include <gtest/gtest.h>
#include "dragon.h"
#include "frog.h"
#include "knight.h"
#include "dungeon.h"

TEST(NPCTest, CreateNPC) {
    Dragon dragon("Dragon1", 10.5, 20.5);
    Frog frog("Frog1", 1.0, 2.0);
    Knight knight("Knight1", 5.0, 5.0);
    
    EXPECT_EQ(dragon.getName(), "Dragon1");
    EXPECT_EQ(frog.getName(), "Frog1");
    EXPECT_EQ(knight.getName(), "Knight1");
}

TEST(NPCTest, Position) {
    Dragon dragon("Draco", 3.14, 2.71);
    
    EXPECT_DOUBLE_EQ(dragon.getX(), 3.14);
    EXPECT_DOUBLE_EQ(dragon.getY(), 2.71);
    
    dragon.setPosition(100.0, 200.0);
    EXPECT_DOUBLE_EQ(dragon.getX(), 100.0);
    EXPECT_DOUBLE_EQ(dragon.getY(), 200.0);
}

TEST(NPCTest, AliveStatus) {
    Frog frog("Froggy", 0, 0);
    
    EXPECT_TRUE(frog.is_alive());
    frog.must_die();
    EXPECT_FALSE(frog.is_alive());
}

TEST(NPCTest, Distance) {
    auto npc1 = std::make_shared<Knight>("K1", 0, 0);
    auto npc2 = std::make_shared<Knight>("K2", 3, 4);
    
    EXPECT_TRUE(npc1->is_close(npc2, 6));
    EXPECT_FALSE(npc1->is_close(npc2, 4));
}

TEST(NPCTest, MoveInsideBorders) {
    Dragon dragon("Movable", 50, 50);
    
    dragon.move(10, -10, 100, 100);
    
    EXPECT_GE(dragon.getX(), 0);
    EXPECT_LE(dragon.getX(), 100);
    EXPECT_GE(dragon.getY(), 0);
    EXPECT_LE(dragon.getY(), 100);
}

TEST(DungeonTest, BasicOperations) {
    Dungeon dungeon;
    
    auto frog = std::make_shared<Frog>("TestFrog", 10, 10);
    
    EXPECT_TRUE(dungeon.addNPC(frog));
    
    auto found = dungeon.findByName("TestFrog");
    EXPECT_NE(found, nullptr);
    EXPECT_EQ(found->getName(), "TestFrog");
    
    EXPECT_TRUE(dungeon.removeByName("TestFrog"));
    EXPECT_EQ(dungeon.findByName("TestFrog"), nullptr);
}

TEST(NPCTest, DifferentTypes) {
    Dragon d("FireDragon", 0, 0);
    Frog f("GreenFrog", 1, 1);
    Knight k("BraveKnight", 2, 2);
    
    EXPECT_EQ(d.getName(), "FireDragon");
    EXPECT_EQ(f.getName(), "GreenFrog");
    EXPECT_EQ(k.getName(), "BraveKnight");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}