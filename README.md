# UE5 Mod Example
This project is a proof of concept for implementing modding support to an unreal engine project.
This is by no means foolproof and meant as a POC. Type of modding may include custom assets, maps, and other assets. A good way to imagine it is like a DLC for your project. (As that is essentially what is happening)
Project SDK can be found here: https://github.com/Ji-Rath/ModingExampleSDK

# Creating a Mod
- Open the SDK project (If you are really picky, it can just be a blank project)
- Create a normal plugin
- Add any content that you would like (ensure the content is isolated and doesnt reference stuff like engine content)
- Package the plugin like DLC
- Find the .pak file in **{ProjectName}/Plugins/{PluginName}/{Saved}/{StagedBuilds}/...** (Keep on opening folders till you reach .pak file)

# Where to Place Pak Files
Once the mod has been packaged into a .pak file, you must place it in the **{ProjectName}/Plugins** folder as that is where files are scanned. It has not been tested whether you can get away with placing them somewhere else.

# Behind The Scenes
After our mod has been packaged into a .pak file, we can begin to import it into the project. This is done by using **MountPakFile** and **RegisterMountPoint**. An example is shown below:
<iframe src="https://blueprintue.com/render/_5fnfz0r/" scrolling="no" allowfullscreen></iframe>

# Accessing Assets
Currently, if you have packaged a map into a .pak and already mounted it, you can open it by using the console command: **open /ModExternal/MapName** (Assuming you have used the example above)

# Additional Notes
- When creating a SDK of your project, consider leaving key blueprint files in the content folder so that modders may use them. Even better if you just give them the whole project.
- There are some specific settings you must follow when packaging (Both the mod and main game). Consider watching this video to ensure you are packaging correctly: https://www.youtube.com/watch?v=ndHNdUSRpho
- SDK Project **needs** to have **Share Material Shader Code** set to false in order for materials to work properly.
- Relevant classes: **UPluginManager, UGameFeaturesSubsystem, IFileManager, UPakFileSubsystem**

# Planning
- ~~Experiment with game features plugin~~
- Add examples which include spawning/replacing assets
- ~~Add UI to load/unload specific mods~~

# Research Notes

## Game Feature Modding
- Using the Game Features plugin and placing mods in the exact path **{ProjectName}/Plugins/GameFeatures** makes things easier. This means you only have to mount the pak files in the plugin, register the mount point, and call **UGameFeaturesSubsystem::LoadAndActivateGameFeaturePlugin**.
  - This is because the project SDK where the mod is packaged is following the same path.
- Using the Game Features plugin to create modular plugins makes mounting much more difficult when not following the exact path. (For example, I want to place mods in **{ProjectName}/Extensions** rather than **{ProjectName}/Plugins/GameFeatures**). This is because when using **UGameFeaturesSubsystem::LoadAndActivateGameFeaturePlugin**, it tries to mount the plugin on its own. This results in the mount path being incorrect.
  - To solve this issue, disable **ExplicitlyLoaded** in the mod's .uplugin file.
  - Manually mount the plugin yourself. Then, mount the pak file, which will then take priority. Finally, call **UGameFeaturesSubsystem::LoadAndActivateGameFeaturePlugin**

# Useful Sources
https://forums.unrealengine.com/t/inside-unreal-modular-game-features/242614/3
https://buckminsterfullerene02.github.io/dev-guide/ModSupport.html
