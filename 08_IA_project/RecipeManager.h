#pragma once
#include <vector>
#include <string>
#include "Recipe.h"

class RecipeManager {
private:
    std::vector<Recipe> recipes;

public:
    void loadRecipesFromCSV(const std::string& filename);
    void saveRecipesToCSV(const std::string& filename);

    void listRecipes() const;
    bool getRecipeById(const std::string& id, Recipe& out) const;

    void addRecipe();
    void updateRecipe();
    void deleteRecipe();
    void searchRecipe() const;

    // 실제 공정 실행 및 추적
    void runRecipeProcess(const std::string& recipeId, class RawMaterial