
#pragma once
#include <chrono>
#include <map>
bool TelekillFW = false;
bool TPPlayer = false;
static bool isWaiting = false;
static std::chrono::steady_clock::time_point targetTime;
bool EnableEsp = false;
bool LogSend = false;
#include "ESP.h"
#include "icon.h"
#include <string>
#include <vector>
#include <deque>
#include <set>
#include <mutex>
#include <algorithm>
#include <cstdint>   
#include <thread>
#include <chrono>
void (*OpenURL)(void*); 

float Lerp(float a, float b, float t) {
    return a + t * (b - a);
}

//BOOL
bool AutoTeleportMark = false;
bool AutoTeleportPlayer = false;

bool EnableFunction = false;
bool EnableResetGuest = false;
bool Time0 = false;
bool AimVisible = true; 
bool AimbotLock = false; 
bool AimbotFiring = false; 
bool AimbotScope = false;  
bool AimTarget = false;
float Fov_Aim = 360.0f;
bool NoReloadStart = false;
bool AimAutofire = false;
bool isFiringState = false;
float delayweapon = 0.0f; 
bool StartRapidfire = false;
bool SilentAimPro = false;
bool AimkillStart = false;
bool AimkillStart2 = false;
bool AimkillStart3 = false;
int SetDamage = 1;

bool EnableGhost = false;
bool FlyAdmin = false;
bool SpeedRun = false;
bool SpeedJoys = false;
bool SpeedTime = false;
float active = 0.0f;
float desactive = 0.0f;
bool saved = false;

bool AutoSwap = false;

bool SuperJump = false;
float TarGetJump = 5.0f;
bool FlyFoldwing = false;
static bool isUnlockSpeedEnabled = false;
bool TestGrappling = false;

bool SmoothFps = false;
bool EnableNightMode = false;
bool skyboxSaved = false;
void* originalSkybox = nullptr;

bool Menu_AutoFlyForward = false;
bool FlyTester = false;
bool TeleMark = false;
bool MagnetAim = false;
bool FlyCarStart = false;
bool FlyTire = false;
bool FlyHover = false;
float Fly_Speed = 0.0f;
float Fly_Height = 5.0f;
bool isInitializedFlyY = false;
Vector3 startPosFlyY = Vector3(0, 0, 0);
bool StartAutoRivive = false;
bool StartAutoExecute = false;
bool ShowBoard = false;
void* savedInstance = nullptr;
Vector3 MarkMapPosition;
int savedType;
uint32_t savedID;
bool InvisibleAWM = false;
bool WalkSpeedToggle = false;
float speedWalkValue = 5.0f; 
bool AutoTPNew = false;
bool TestGilde1 = false;
bool TestGilde2 = false;
bool FlyTelePLUS = false;
bool Hidedamage = true;
bool ForceState = false;
static int selectedState = 0;
static int selectedPose = 0;
const char* state_items[] = {
    "Walking", "Falling", "Parachuting", "OnBoard", "SkyDiving", 
    "HitFly", "Swimming", "OnStrop", "Football", "JetFly", 
    "Gliding", "OnGrapplingHook", "Skateboarding", "OnFerrisWheel", "FlightRoam", 
    "FaithJumping", "Swing", "Sprint", "SlideRunning", "SlideFalling"
};
const char* pose_items[] = {
    "STANDING", "CROUCHING", "DASHING", "CREEPING", "SWIMDASH",
    "SKYDIVING", "CROSSOVER", "CLIMBOVER", "KNOCKDOWN", "SWIMSURFING",
    "HIGHFALLING", "LOWFALLING", "JETFLYRUSH", "SECONDFALLING", "SITTING",
    "CATAPULTFALLING", "CATAPULTSTANDING", "GLIDING", "GLIDEFALLING", "FOUNTAINFALLING",
    "HUMANTIREFALLING", "PARTYDANCE", "FLIGHTROAMRUSH", "CANNONSTANDING", "PLATFORM_FALLING",
    "JUMPPADFALLING", "STROPFALLING", "SLIDE_OFF_FALLING", "DASH_FALLING", "FAST_FALLING",
    "SNOWSLIDE_GRAB", "PARACHUT_FALLING", "SWIMMINGSURFDASH", "GB_FOOTBALL_FLY", "BT_TELEPORT"
};

static void SwitchPhysXState(void *player) {
    if (player == NULL) return;
    if(ForceState){
          ((void* (*)(void*, int, int))(offset_SwitchPhysXState))(player, selectedState, selectedPose);
    }
    if(AutoTeleportMark){
          ((void* (*)(void*, int, int))(offset_SwitchPhysXState))(player, 1, 11);
    }
    if(FlyAdmin){
          ((void* (*)(void*, int, int))(offset_SwitchPhysXState))(player, 1, 11);
    }
}

//SAVEPOSITION MARK
void (*old_SendMapMarkChanged)(void* instance, Vector3 realPos, bool isDel, int pointType, uint32_t levelObjectID, bool playMarkSound);
void _SendMapMarkChanged(void* instance, Vector3 realPos, bool isDel, int pointType, uint32_t levelObjectID, bool playMarkSound) {
    savedInstance = instance;
    MarkMapPosition = realPos;
    savedType = pointType;
    savedID = levelObjectID;
    old_SendMapMarkChanged(instance, realPos, isDel, pointType, levelObjectID, playMarkSound);
}


//LOOP PLAYER
bool isEnemyInRangeWeapon(void *player, void *enemy, void* weapon)
{
    if (!player || !enemy || !weapon) return false;
    return Vector3::Distance(GetHeadPosition(player), GetHeadPosition(enemy)) <= get_Range(weapon);
}

bool Visible_Check(void* enemy) {
    void* cam = Camera_main();
    void* head = enemy ? Player_GetHeadCollider(enemy) : nullptr;
    if (!cam || !head) return false;
    void* hit = nullptr;
    return !Physics_Raycast(Transform_GetPosition(Component_GetTransform(cam)), Transform_GetPosition(Component_GetTransform(head)), 12, &hit);
}

void* EnemyVisible(void* match) {
    void* LocalPlayer = match ? GetLocalPlayer(match) : nullptr;
    void* cam = Camera_main();
    auto dict = match ? *(monoDictionary<void*, void*>**)((uintptr_t)match + ListPlayer) : nullptr;
    if (!LocalPlayer || !cam || !dict || (uintptr_t)dict < 0x1000000 || !dict->entries) return nullptr;
    
    int capacity = dict->getCapacity();
    if (capacity <= 0 || capacity > 2000) return nullptr;

    Vector3 camPos = CameraMain(LocalPlayer); 
    Vector3 camForward = GetForward(Component_GetTransform(cam));

    float shortestAngle = 99999.0f;
    float maxAngle = Fov_Aim;  // ใช้ตรงๆ ไม่ต้อง /2
    void* closestEnemy = nullptr;

    for (int i = 0; i < capacity; i++) {
        auto& entry = dict->getEntry(i);
        if (entry.hashCode < 0) continue;
        void* Player = entry.value;
        if (!Player || (uintptr_t)Player < 0x1000000 || Player == LocalPlayer || 
            get_isLocalTeam(Player) || get_IsDieing(Player) || !get_MaxHP(Player)) continue;

        Vector3 enemyHead = GetHeadPosition(Player);
        Vector3 dirToEnemy = Vector3::Normalized(enemyHead - camPos);

        // ใช้ Dot Product แทน Angle
        float dot = dirToEnemy.x * camForward.x + 
                    dirToEnemy.y * camForward.y + 
                    dirToEnemy.z * camForward.z;
        
        // แปลง Dot Product เป็นมุม (องศา)
        float angle = acosf(dot) * 57.295779513f; // 180/PI

        if (angle > maxAngle) continue;

        if (AimVisible && !Visible_Check(Player)) continue;

        if (angle < shortestAngle) {
            shortestAngle = angle;
            closestEnemy = Player;
        }
    }
    return closestEnemy;
}

//SWAP WEAPON
static void Syns_SwapWeapon_Impl(void *LocalPlayer, void *WeaponOnHand) {
    if (!LocalPlayer || !WeaponOnHand) return;

    static size_t off_pID = 0;
    static size_t field_IHAAMHPPLMG = 0;
    static size_t field_DGLCOGJJFM = 0;
    static uintptr_t method_Send = 0;

    if (off_pID == 0) {
        off_pID           = Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("MHGFKKALMDK"));
        field_IHAAMHPPLMG = Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("message"), OBFUSCATE("EOBFCLNNIJH"), OBFUSCATE("BEADLMGGGGL"));
        field_DGLCOGJJFM  = Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("message"), OBFUSCATE("EOBFCLNNIJH"), OBFUSCATE("FIKKBFDEJOK"));
        method_Send       = (uintptr_t)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("GameFacade"), OBFUSCATE("Send"), 4);
    }

     void *RUDP_CHANGE_INVENTORY_ON_HAND = Il2CppCreateClassInstance(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("message"), OBFUSCATE("EOBFCLNNIJH"));
    if (RUDP_CHANGE_INVENTORY_ON_HAND) {
        uint32_t pID = *(uint32_t*)((uintptr_t)LocalPlayer + off_pID);
        *(uint32_t * )((uint64_t) RUDP_CHANGE_INVENTORY_ON_HAND + field_IHAAMHPPLMG) = pID;
        *(uint32_t * )((uint64_t) RUDP_CHANGE_INVENTORY_ON_HAND + field_DGLCOGJJFM) = GetUniqueID(WeaponOnHand);
        if (method_Send) {
            ((bool (*)(uint32_t, void*, uint8_t, bool))method_Send)(108, RUDP_CHANGE_INVENTORY_ON_HAND, 2, false);
        }
    }
}

//AIMSILENT
int (*old_BLAGCMCGEJG1)(void* ist, GMPGMPFNMFP* HitObject);
int BLAGCMCGEJG1(void* ist, GMPGMPFNMFP* HitObject) { 
    if (SilentAimPro && HitObject != nullptr) {
        void* current_match = Curent_Match();
        void* local_player  = GetLocalPlayer(current_match);
        void* closest_enemy = EnemyVisible(current_match);
        void* camera        = Camera_main();      
        void* target        = closest_enemy; 
        void* weapon_hand   = GetWeaponOnHand(local_player);
        if (local_player && target && GetHp(target) > 0 && isEnemyInRangeWeapon(local_player, target, weapon_hand)) {            
            Vector3 EnemyLocation  = (SetDamage == 1) ? GetHeadPosition(target) : GetHipPosition(target);
            Vector3 PlayerLocation = GetHeadPosition(local_player);
            HitObject->HLIJMDODPIM = get_gameObject(Player_GetHeadCollider(target));
            HitObject->OCEBCHENIOK = Player_GetHeadCollider(target);
            HitObject->MBGBCLNJOMK = EnemyLocation;
            HitObject->DGFLGBEOGPG = Vector3(0, 1, 0);
            HitObject->IKDEGKIICJP = Vector3::Normalized(EnemyLocation - PlayerLocation);
            HitObject->LMAEGPEAECO = PlayerLocation;
            HitObject->FLCLOHCBJEI = (int32_t)1;
            HitObject->DEDOKPCAHAC = (SetDamage == 1);
            HitObject->GGJOADOBLID = false;
            if (camera) {
                Quaternion PlayerLook = GetRotationToLocation(EnemyLocation, 0.1f, PlayerLocation);
                set_aim(local_player, PlayerLook);
            }
        }
    }
    return old_BLAGCMCGEJG1(ist, HitObject);
}

float SilentAimDelay = 0.8f; 
float DamageMult = 1.0f; 
int AutoKillMode = 0;
int AimTarget1 = 1;
static void* g_Cached_ist = nullptr;

//AUTOKILL
void StartAimKillXNEW(void* enemy) {
    void* match = Curent_Match();
    if (!match || !enemy || get_IsDieing(enemy)) return;
    void* local = GetLocalPlayer(match);
    if (!local || get_IsDieing(local)) return;
    
    void* weapon = GetWeaponOnHand(local);
    if (!weapon) return;
	
    int baseDamage = *(int*)((uintptr_t)weapon + offset_BaseDamage);
    if (baseDamage <= 0) baseDamage = 100; 
    int finalDamage = (int)(baseDamage * DamageMult);
	
    Vector3 targetPos = (AimTarget1 == 1) ? GetHeadPosition(enemy) : GetHipPosition(enemy);
	if (targetPos.x == 0 && targetPos.y == 0 && targetPos.z == 0) return;
    Vector3 origin = GetHeadPosition(local);
    if (origin.x == 0 && origin.y == 0 && origin.z == 0) return;

    void* ist = g_Cached_ist ? g_Cached_ist : weapon;
            
    void* hitObj = Il2CppCreateClassInstance(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("CGKJLKPMGDJ"));
    if (!hitObj) return;
	
    GMPGMPFNMFP* hitData = (GMPGMPFNMFP*)hitObj;
    memset((char*)hitData + 16, 0, sizeof(GMPGMPFNMFP) - 16); 

    hitData->m_IsInPool = false;                                         
    hitData->HLIJMDODPIM = get_gameObject(Player_GetHeadCollider(enemy)); 
    hitData->OCEBCHENIOK = Player_GetHeadCollider(enemy);                
    hitData->MBGBCLNJOMK = targetPos;                                    
    hitData->DGFLGBEOGPG = targetPos;                                    
    hitData->IKDEGKIICJP = Vector3::Normalized(targetPos - origin);      
    hitData->LMAEGPEAECO = origin;                                       
    
    hitData->BGKGJKDILEA = 200;                                   
    hitData->PGCPFOAJHBM = Vector3::Distance(origin, targetPos);         
    hitData->CMPLENABKPC = 0;                                            
    hitData->FLCLOHCBJEI = (int32_t)AimTarget1; // แก้ตรงนี้

    hitData->CICAFKKFBDL = nullptr;                                      
    hitData->DEDOKPCAHAC = false;                                        
    hitData->GGJOADOBLID = false;                                        
    hitData->KPEICEMCHIF = origin;                                       
    hitData->LNOIFBAFGOK = 0;                                            
    hitData->GBOLEJPOAEP = nullptr;                                      
    hitData->EMGLFDCDEBG = nullptr;

    if (Camera_main()) {
        set_aim(local, GetRotationToLocation(targetPos, 0.1f, origin));
        StartFiring(local, weapon);
        old_BLAGCMCGEJG1((void*)ist, hitData); 
        StopFire1(local, weapon);
        Syns_SwapWeapon_Impl(local, weapon);
    }
}
//HITFLY
float GUI_FlyTime = 1.0f;
float GUI_FlySpeedXZ = 25.0f;
float GUI_FlySpeedY = 20.0f;
void ExecuteHitFly() {
    void* CurrentMatch = Curent_Match();
    void* local_player = GetLocalPlayer(CurrentMatch);
    void* mainCam = Camera_main();
    if (!local_player || !mainCam) return;

    static size_t off_IHAAM, off_CJK, off_HIF, off_DGDI, off_MHC;
    if (off_IHAAM == 0) {
        off_IHAAM = Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("message"), OBFUSCATE("NNDOIBJBAJI"), OBFUSCATE("BEADLMGGGGL"));
        off_CJK   = Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("message"), OBFUSCATE("NNDOIBJBAJI"), OBFUSCATE("DCEMJOGHMEG"));
        off_HIF   = Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("message"), OBFUSCATE("NNDOIBJBAJI"), OBFUSCATE("KJGOMDJCKEB"));
        off_DGDI  = Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("message"), OBFUSCATE("NNDOIBJBAJI"), OBFUSCATE("DPJAIECLHGL"));
        off_MHC   = Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("message"), OBFUSCATE("NNDOIBJBAJI"), OBFUSCATE("MGJNBIJNLIJ"));
    }

    void *msg = Il2CppCreateClassInstance(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("message"), OBFUSCATE("NNDOIBJBAJI"));
    if (!msg) return;

    uintptr_t addr = (uintptr_t)msg; 
    uint32_t pID = *(uint32_t*)((uintptr_t)local_player + Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("MHGFKKALMDK")));
    
    *(uint32_t*)(addr + off_IHAAM) = pID;
    *(float*)(addr + off_CJK) = GUI_FlyTime;
    *(float*)(addr + off_HIF) = GUI_FlySpeedXZ;
    *(float*)(addr + off_DGDI) = GUI_FlySpeedY;

    static uintptr_t off_Pack = (uintptr_t)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("message"), OBFUSCATE("LMCBHLCPAJD"), OBFUSCATE("MGOGJCMCIPG"), 1);
    Vector3 fwd = GetForward(Component_GetTransform(mainCam));
    void* packedObj = ((void* (*)(Vector3))off_Pack)(fwd); 
    
    if (packedObj) {
        *(void**)(addr + off_MHC) = packedObj;
    }
	
    static uintptr_t off_Send = (uintptr_t)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("GameFacade"), OBFUSCATE("Send"), 4);
    ((bool (*)(uint32_t, void*, uint8_t, bool))off_Send)(657, msg, 2, false);
}

void fastreload() {
    void* CurrentMatch = Curent_Match();
    void* local_player = GetLocalPlayer(CurrentMatch);
    if (local_player != nullptr) {
        void *playerattributes = *(void **) ((uint64_t) local_player + _playerAttributes);
        if (playerattributes != nullptr) {
            if (NoReloadStart) {
                *(bool *) ((uintptr_t) playerattributes + offset_NoReload) = true;
            } else {
                *(bool *) ((uintptr_t) playerattributes + offset_NoReload) = false;
            }
        }
    }
}


bool isAutoKillRunning = false;

void ProcessAutoKill(void* local_player, void* CurrentMatch) {
    void* WeaponHand = GetWeaponOnHand(local_player);
    if (!WeaponHand) {
        isAutoKillRunning = false;
        return;
    }
    auto dict = CurrentMatch ? *(monoDictionary<void*, void*>**)((uintptr_t)CurrentMatch + ListPlayer) : nullptr;
    if (!local_player || !dict || (uintptr_t)dict < 0x1000000 || !dict->entries) {
        isAutoKillRunning = false;
        return;
    }
    int capacity = dict->getCapacity();
    if (capacity <= 0 || capacity > 2000) {
        isAutoKillRunning = false;
        return;
    }
    bool foundAnyEnemy = false;
    for (int i = 0; i < capacity; i++) {
        auto& entry = dict->getEntry(i);
        if (entry.hashCode < 0) continue;
        void* enemy = entry.value;
        if (!enemy || (uintptr_t)enemy < 0x1000000 || enemy == local_player || 
            get_isLocalTeam(enemy) || get_IsDieing(enemy) || !get_MaxHP(enemy)) continue;
        if (!isEnemyInRangeWeapon(local_player, enemy, WeaponHand)) continue;
        if (AimVisible && !Visible_Check(enemy)) continue;
        foundAnyEnemy = true;
        if (AimkillStart) {
            StartAimKillXNEW(enemy); 
        }
    }
    if (foundAnyEnemy && AimkillStart) {
        isAutoKillRunning = true;
    } else {
        isAutoKillRunning = false;
    }
}

//AIMBOT
void ProcessAimbot(void* local_player, void* CurrentMatch, void* closestEnemy) {
    if (!closestEnemy) return;
    bool IsFiring = get_IsFiring(local_player);
    bool isScoped = get_IsSighting(local_player);
    bool shouldAim = false;
    
    if (AimbotLock) {
        shouldAim = true;
    }
    if (AimbotFiring && IsFiring) {
        shouldAim = true;
    }
    if (AimbotScope && isScoped) {
        shouldAim = true;
    }
    if (!shouldAim) return;

    static Vector3 lastEnemyPos = Vector3(0, 0, 0);
    Vector3 headPos = GetHeadPosition(closestEnemy);
    Vector3 bodyPos = getPosition(closestEnemy);
    
    if (headPos.x == 0 && headPos.y == 0 && headPos.z == 0) return;
    if (bodyPos.x == 0 && bodyPos.y == 0 && bodyPos.z == 0) return;

    bool isTargetMoving = (lastEnemyPos.x != 0 || lastEnemyPos.y != 0 || lastEnemyPos.z != 0) && Vector3::Distance(bodyPos, lastEnemyPos) > 0.03f;
    Vector3 predictedHeadPos = headPos;
    
    if (isTargetMoving) {
        Vector3 headVelocity = headPos - Vector3(lastEnemyPos.x, lastEnemyPos.y + (headPos.y - bodyPos.y), lastEnemyPos.z);
        predictedHeadPos = headPos + (headVelocity * 0.05f);
    }
    lastEnemyPos = bodyPos;
    Vector3 targetPosition = AimTarget ? bodyPos : predictedHeadPos;

    void* camera = Camera_main();
    if (camera && Component_GetTransform(camera)) {
        set_aim(local_player, GetRotationToLocation(targetPosition, 0.1f, CameraMain(local_player)));
    }
}


//AUTOFIRE
void ProcessAutoFire(void* local_player, void* CurrentMatch, void* closestEnemy) {
    void* WeaponHand = GetWeaponOnHand(local_player);
    if (!WeaponHand) return;
    bool isVisible = false;
    if (AimAutofire && closestEnemy) {
        isVisible = Visible_Check(closestEnemy);
    }
    bool enemyInRange = isEnemyInRangeWeapon(local_player, closestEnemy, WeaponHand);
    bool hasTarget = (closestEnemy && enemyInRange);
    bool shouldFire = (hasTarget && AimAutofire);
    static auto lastFireTime = std::chrono::steady_clock::now();
    float weaponDelay = get_Delay(WeaponHand);
    if (weaponDelay < delayweapon) weaponDelay = delayweapon;
    if (shouldFire) {
        static Vector3 lastEnemyPos = Vector3(0, 0, 0);
        Vector3 headPos = GetHeadPosition(closestEnemy);
        Vector3 bodyPos = getPosition(closestEnemy);
        if (headPos.x != 0 && headPos.y != 0 && headPos.z != 0 &&
            bodyPos.x != 0 && bodyPos.y != 0 && bodyPos.z != 0) {
            bool isTargetMoving = (lastEnemyPos.x != 0 || lastEnemyPos.y != 0 || lastEnemyPos.z != 0) && Vector3::Distance(bodyPos, lastEnemyPos) > 0.03f;
            Vector3 predictedHeadPos = headPos;
            if (isTargetMoving) {
                Vector3 headVelocity = headPos - Vector3(lastEnemyPos.x, lastEnemyPos.y + (headPos.y - bodyPos.y), lastEnemyPos.z);
                predictedHeadPos = headPos + (headVelocity * 0.05f);
            }
            lastEnemyPos = bodyPos;
            Vector3 targetPosition = AimTarget ? bodyPos : predictedHeadPos;
            void* camera = Camera_main();
            if (camera && Component_GetTransform(camera)) {
                set_aim(local_player, GetRotationToLocation(targetPosition, 0.1f, CameraMain(local_player)));
            }
        }
        auto now = std::chrono::steady_clock::now();
        std::chrono::duration<float> elapsed = now - lastFireTime;
        if (elapsed.count() >= weaponDelay) {
            StartFiring(WeaponHand);
            lastFireTime = now;
            isFiringState = true;
        }
    } else if (isFiringState) {
        StopFiring(WeaponHand);
        isFiringState = false;
    }
}

void UpdateRapidFire() {
    if (!StartRapidfire) return;
    void* current_match = Curent_Match();
    if (!current_match) return;
    void* local_player = GetLocalPlayer(current_match);
    if (!local_player) return;
	void* WeaponHand = GetWeaponOnHand(local_player);
    if (!WeaponHand) return;
    bool IsFiring = get_IsFiring(local_player);
    static float fireTimer1 = 0.0f;
    static bool isFiringState1 = false;
    fireTimer1 += ImGui::GetIO().DeltaTime;
    float weaponDelay = get_Delay(WeaponHand);
    if (weaponDelay < delayweapon) weaponDelay = delayweapon;
    if (StartRapidfire && IsFiring) {
        if (fireTimer1 >= weaponDelay) {
            StartFiring(WeaponHand);
            fireTimer1 = 0.0f;
            isFiringState1 = true;
        }
    } else if (isFiringState1) {
        StopFiring(WeaponHand);
        isFiringState1 = false;
    }
}

//CREDIT IN GAME ( UI SCENCE )
void CreditText() {
    void* currentMatch = Curent_Match();
    if (!currentMatch) return;
    void* localPlayer = GetLocalPlayer(currentMatch);
    if (!localPlayer) return;
    static auto lastTipTime = std::chrono::steady_clock::now();
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTipTime).count();
    if (elapsed >= 1500) {
        static monoString* creditStr = il2cpp_string_new("[ BS4484 CHEATS ]");
        ShowCenterUpTeammateTips(creditStr, 2.0f);
        lastTipTime = now;
    }
}


//ESP ALERT
bool EspAlert = false;
void* GetClosestEnemy(void* match) {
    void* LocalPlayer = match ? GetLocalPlayer(match) : nullptr;
    auto dict = match ? *(monoDictionary<void*, void*>**)((uintptr_t)match + ListPlayer) : nullptr;
    if (!LocalPlayer || !dict || (uintptr_t)dict < 0x1000000 || !dict->entries) return nullptr;
    int capacity = dict->getCapacity();
    if (capacity <= 0 || capacity > 2000) return nullptr;
    Vector3 localPos = getPosition(LocalPlayer);
    float shortestDistance = 99999.0f;
    void* closestEnemy = nullptr;
    for (int i = 0; i < capacity; i++) {
        auto& entry = dict->getEntry(i);
        if (entry.hashCode < 0) continue;
        void* Player = entry.value;
        if (!Player || (uintptr_t)Player < 0x1000000 || Player == LocalPlayer || 
            get_isLocalTeam(Player) || get_IsDieing(Player) || !get_MaxHP(Player)) continue;
        float distance = Vector3::Distance(localPos, getPosition(Player));
        if (distance < shortestDistance) {
            shortestDistance = distance;
            closestEnemy = Player;
        }
    }
    return closestEnemy;
}
void EspAlertStart() {
    if (!EnableEsp || !EspAlert) return;
    void* uiInstance = CurrentInGameUIScene();  
    if (uiInstance == nullptr) return;  
    void* match = Curent_Match();  
    if (!match) return;  
    void* localPlayer = GetLocalPlayer(match);  
    if (!localPlayer) return;  
    void* enemy = GetClosestEnemy(match);  
    if (!enemy) return;  
    if (enemy == localPlayer) return;  
    if (get_isLocalTeam(enemy)) return;  
    if (get_IsDieing(enemy)) return;  
    float distance = Vector3::Distance(getPosition(localPlayer), getPosition(enemy));  
    int curHp = GetHp(enemy);  
    int maxHp = get_MaxHP(enemy);  
	monoString* nameMono = get_NickName(enemy);  
    std::string weaponName = "None";  
    void* weapon = GetWeaponOnHand(enemy);  
    if (weapon) {  
        int weaponID = GetUniqueID(weapon);  
        weaponName = GetWeaponNameByID(weaponID);  
    }  
    monoString* infoLine = U3DStrFormat(distance, curHp, maxHp, weaponName.c_str());  
    ShowAssistantText(uiInstance, nameMono, infoLine);
}


float FlyAdminHeight = 10.0f;
float Fly_AdminSpeed = 8.0f;
float Fly_AdminSpeedY = 8.0f;
//FLY HACK
void ProcessMovement(void* local_player) {
    if (!FlyTire && !FlyHover && !FlyCarStart && !FlyTester && !FlyTelePLUS && !FlyAdmin) {
        isInitializedFlyY = false;
        return; 
    }

    void* trLocal = Component_GetTransform(local_player);
    if (!trLocal) return;

    Vector3 myPos = Transform_INTERNAL_GetPosition(trLocal);
    void* camera = Camera_main();
    void* trCamera = camera ? Component_GetTransform(camera) : nullptr;
    if (!trCamera) return;

    static auto lastFrameTime = std::chrono::steady_clock::now();
    auto now = std::chrono::steady_clock::now();
    float dt = std::chrono::duration<float>(now - lastFrameTime).count();
    lastFrameTime = now;
    
    if (dt > 0.1f) dt = 0.016f;

    if (!isInitializedFlyY) {
        startPosFlyY = myPos;
        isInitializedFlyY = true;
    }

    Vector3 camForward = GetForward(trCamera);
    camForward.y = 0; 

    if (FlyCarStart) {
        void* currentVehicle = GetVehicleIAmIn(local_player);
        if (currentVehicle) {
            void* trVehicle = Component_GetTransform(currentVehicle);
            if (trVehicle) {
                Vector3 vPos = Transform_INTERNAL_GetPosition(trVehicle); 
                
                float carNextX = vPos.x + (camForward.x * Fly_Speed * dt);
                float carNextZ = vPos.z + (camForward.z * Fly_Speed * dt);
                float targetCarY = startPosFlyY.y + Fly_Height;
                float carNextY = Lerp(vPos.y, targetCarY, 3.0f * dt); 

                Transform_INTERNAL_SetPosition(trVehicle, Vvector3(carNextX, carNextY, carNextZ));
                return;
            }
        }
    }

    float nextX = myPos.x;
    float nextY = myPos.y;
    float nextZ = myPos.z;

    if (FlyTire) {
        nextX = myPos.x + (camForward.x * 0.0f * dt);
        nextZ = myPos.z + (camForward.z * 0.0f * dt);
        float targetY = startPosFlyY.y + 100.0f;
        nextY = Lerp(myPos.y, targetY, 1.0f * dt); 
    } 
    if (FlyHover) {
        nextX = myPos.x + (camForward.x * 0.0f * dt);
        nextZ = myPos.z + (camForward.z * 0.0f * dt);
        float targetY = startPosFlyY.y + 8.0f;
        nextY = Lerp(myPos.y, targetY, 2.0f * dt); 
    }  
    if (FlyTelePLUS) {
        nextX = myPos.x;
        nextZ = myPos.z;
        float targetY = startPosFlyY.y + 5.0f; 
        
        nextY = Lerp(myPos.y, targetY, 6.0f * dt); 
        if (abs(nextY - targetY) < 0.05f) {
            nextY = targetY;
        }
    }  
    if (FlyAdmin) {
        nextX = myPos.x + (camForward.x * Fly_AdminSpeed * dt);
        nextZ = myPos.z + (camForward.z * Fly_AdminSpeed * dt);
  
        float targetY = startPosFlyY.y + FlyAdminHeight;

        float riseSpeed = Fly_AdminSpeedY; 
        if (myPos.y < targetY) {
            nextY = myPos.y + (riseSpeed * dt);
            if (nextY > targetY) nextY = targetY; 
        } else if (myPos.y > targetY) {
            nextY = myPos.y - (riseSpeed * dt);
            if (nextY < targetY) nextY = targetY;
        } else {
            nextY = targetY;
        }
    }  
    if (TestGilde1) {
        nextX = myPos.x + (camForward.x * Fly_Speed * dt);
        nextZ = myPos.z + (camForward.z * Fly_Speed * dt);
        float targetY = startPosFlyY.y + Fly_Height;
        nextY = Lerp(myPos.y, targetY, 2.0f * dt); 
    }  
    Transform_INTERNAL_SetPosition(trLocal, Vvector3(nextX, nextY, nextZ));
}

bool TestStartOnGrapplingHook = false;
//GAME LOOP
void (*Gameupdate)(void *Player);
void _Gameupdate(void *Player) {
    if (!Player) { Gameupdate(Player); return; }
    
    void* CurrentMatch = Curent_Match();
    if (!CurrentMatch) { Gameupdate(Player); return; }
   
    void* local_player = GetLocalPlayer(CurrentMatch);
    if (!local_player) { Gameupdate(Player); return; }

    if (Player == local_player) {
        CreditText();
		EspAlertStart();
        if (EnableFunction) {
			if (NoReloadStart){
				fastreload();
			}
		    if (ForceState) {
        		SwitchPhysXState(Player); 
   		    }
		    if (FlyAdmin) {
        		SwitchPhysXState(Player); 
   		    }
		    if (AutoTeleportMark) {
        		SwitchPhysXState(Player); 
   		    }
    		if (TestGilde1){
				StartSAPGlid(Player);
			}
			if (TestGilde2){
				StartSAPJumpBeforeGlid(Player);
			}
			if (TestStartOnGrapplingHook){
				StartOnGrapplingHook(Player);
			}
			if (AutoTPNew) {
    			if (!isWaiting) {
        			StartSAPJumpBeforeGlid(Player);
        			targetTime = std::chrono::steady_clock::now() + std::chrono::seconds(3);
        			isWaiting = true;
    			} 
    			else
				{
					if (std::chrono::steady_clock::now() >= targetTime) {
        				StartSAPGlid(Player);
						isWaiting = false;
					}
				}
			}
			if (TelekillFW){
				StartSAPJumpBeforeGlid(Player);
				StartSAPGlid(Player);
			}
			if (AutoSwap){
			    static auto lastSwitch = std::chrono::steady_clock::now();
    			auto now = std::chrono::steady_clock::now();
    			if (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastSwitch).count() < 50) return; // 50ms — inline swap in StartAimKillSend handles instant swap already
    			lastSwitch = now;
    			if (!Player || get_IsDieing(Player)) return;
    			void *weaponOnHand = GetWeaponOnHand(Player);
    			if (!weaponOnHand) return;
    			static int lastSlot = 2;
    			int newSlot = (lastSlot == 1) ? 2 : 1;
    			SwapWeapon(Player, newSlot, 1);
  			    lastSlot = newSlot;
    			void *syncedWeapon = GetWeaponOnHand(Player);
    			if (syncedWeapon) Syns_SwapWeapon_Impl(Player, syncedWeapon);
			}
            void* targetEnemy = EnemyVisible(CurrentMatch);
            ProcessAimbot(local_player, CurrentMatch, targetEnemy);
            ProcessAutoFire(local_player, CurrentMatch, targetEnemy);
            ProcessAutoKill(local_player, CurrentMatch);
            ProcessMovement(local_player);
			UpdateRapidFire();
        }
    }
    Gameupdate(Player);
}
bool FixtelePortNew = false;
float TimeSpeed = 1.85f;
float testhiegh = 3.0f;
bool TeleportUp3F = false;
//MAIN LOOP
void update() {
	void* CurrentMatch = Curent_Match();
    if (!CurrentMatch) return;
   
    void* local_player = GetLocalPlayer(CurrentMatch);
    if (!local_player) return;
	
	void* trLocal = Component_GetTransform(local_player);
    if (!trLocal) return;
    
    Vector3 myPos = Transform_INTERNAL_GetPosition(trLocal);
	
	if (!EnableFunction) return;
	if (FixtelePortNew) {
    	float newY = myPos.y + testhiegh;
    	Transform_INTERNAL_SetPosition(trLocal, Vvector3(myPos.x, newY, myPos.z));
    	FixtelePortNew = false;
	}
    if (EnableNightMode) {
        if (!skyboxSaved) {
            originalSkybox = get_skybox_Original();
            skyboxSaved = true;
        }
        set_skybox_Null(nullptr);
        set_ambientMode_Black(3);
    } else {
        if (skyboxSaved && originalSkybox) {
            set_skybox_Null(originalSkybox);
            set_ambientMode_Black(0);
            skyboxSaved = false;
            originalSkybox = nullptr;
        }
    }
	if (Menu_AutoFlyForward){
		ExecuteHitFly();
		Menu_AutoFlyForward = false;
	}
	if (TeleMark) {
        float newY = (myPos.y - 0.5f < 0.0f) ? 0.0f : myPos.y - 0.5f;
        Transform_INTERNAL_SetPosition(trLocal, Vvector3(MarkMapPosition.x, newY, MarkMapPosition.z));
		TeleMark = false;
    }
void* Simulation = GetSimulationTimer();
if (Simulation != nullptr) {
    float FixedDeltaTime = GetTimer(Simulation);
    if (!saved) {
        active = FixedDeltaTime * TimeSpeed;
        desactive = FixedDeltaTime;
        saved = true;
    }
    
    // รวมเงื่อนไขการควบคุม Timer ไว้ที่เดียว
    if (AutoTeleportMark && EnableFunction) {
        if (FixedDeltaTime != 0.0f) {
            SetTimer(Simulation, 0.0f);
        }
    } else if (isAutoKillRunning && EnableFunction) {
        if (FixedDeltaTime != 0.0f) {
            SetTimer(Simulation, 0.0f);
        }
    } else if (SpeedTime && EnableFunction) {
        if (FixedDeltaTime != active) {
            SetTimer(Simulation, active);
        }
    } else {
        if (FixedDeltaTime != desactive) {
            SetTimer(Simulation, desactive); 
        }
    }
}

}


//OTHER HOOK
bool (*ResetGuest)(void* _this);
bool _ResetGuest(void* _this) {
    if (EnableResetGuest && EnableFunction) {
        return true; 
    }
    return ResetGuest(_this);
}

//SPEED RUN
float (*old_GetCurrentDashSpeed)(void *instance);
float hook_GetCurrentDashSpeed(void *instance) {
    void* localPlayer = GetLocalPlayer(Curent_Match());
    if (instance != nullptr && instance == localPlayer) {
        if (SpeedRun && EnableFunction) {
            return 9.0f; 
        }
    }
    return old_GetCurrentDashSpeed(instance);
}

//SUPERJUMP
float (*orig_get_MaxJumpHeight)(void *instance);
float my_get_MaxJumpHeight(void *instance) {
    if (instance != NULL) {
        if (SuperJump && EnableFunction) {
            return TarGetJump;
        }
        if (FlyAdmin && EnableFunction) {
            return 20.0f;
        }
    }
    return orig_get_MaxJumpHeight(instance);
}

bool GravityHack = false;
float (*orig_get_CustomGravity)(void *instance);
float my_get_CustomGravity(void *instance) {
    if (instance != NULL) {
		if (SuperJump && EnableFunction) {
            return 0.0f;
        }
		if (FlyAdmin && EnableFunction) {
            return 0.0f;
        }
		if (FlyHover && EnableFunction) {
            return 0.0f;
        }
		if (GravityHack && EnableFunction) {
            return 0.0f;
        }
	}
    return orig_get_CustomGravity(instance);
}

float (*orig_get_RisingGravity)(void *instance);
float my_get_RisingGravity(void *instance) {
    if (instance != NULL) {
        if (SuperJump && EnableFunction) {
            return -0.9f;
        }
        if (FlyAdmin && EnableFunction) {
            return -0.9f;
        }
	}
    return orig_get_RisingGravity(instance);
}

static int current_selection = 0; 
int ForcePhysXState = -1; // -1 = ปิด (ใช้ค่าปกติ), 0-19 = Force สถานะนั้นๆ
const char* PhysXStates[] = { 
    "Disable Force", "Walking", "Falling", "Parachute", "OnBoard", "SkyDiving", 
    "Swimming", "UnderWater", "Football", "JetFly", "Foldwing", "Grappling", 
    "Skateboard", "FerrisWheel", "FlightRoam", "FaithJumping", "Swing", 
    "Sprint", "SlideRunning", "SlideFalling" 
};

int (*OSYN_GetPhysXState)(void *Player, void* ClosestEnemy);
int HSYN_GetPhysXState(void *Player, void* ClosestEnemy)
{
	void* CurrentMatch = Curent_Match();
    void* local_player = GetLocalPlayer(CurrentMatch);
    if(Player == local_player){
		if (ForcePhysXState != -1) return ForcePhysXState;
        if (SuperJump && EnableFunction)        return 13; 
		if (FlyHover && EnableFunction)        return 13; 
		if (AutoTeleportMark && EnableFunction)        return 13; 
		if (FlyAdmin && EnableFunction)        return 13; 
	}
    return OSYN_GetPhysXState(Player, ClosestEnemy);
}

//SMOOTH FPS
bool (*CachedSmoothHighFrame)();
bool _CachedSmoothHighFrame() {
	if(SmoothFps && EnableFunction){
		return true; 
	}
    return CachedSmoothHighFrame(); 
}

bool (*IsFoldWingGliding)(void *player);
bool _IsFoldWingGliding(void *player) {
    void* CurrentMatch = Curent_Match();
    if (CurrentMatch != nullptr) {
        void* LocalPlayer = GetLocalPlayer(CurrentMatch);
        if (LocalPlayer != nullptr) {
            if (player != NULL) {
                if (EnableFunction && SpeedJoys) {
                    return true;
                }
            }
        }
    }
    return IsFoldWingGliding(player);
}

bool (*orig_SpeedBypass)(void* instance);
bool hook_SpeedBypass(void* instance) {
    if (EnableFunction && SpeedJoys) {
        return true;
    }
    return orig_SpeedBypass(instance);
}

bool (*orig_IsPoseFallingHigh)(void *Player);
bool hook_IsPoseFallingHigh(void *Player){
    if (FlyHover && Player) { return true; }
	if (FlyTester && Player) { return true; }
	if (FlyTelePLUS && Player) { return true; }
	if (AutoTeleportMark && Player) { return true; }
	if (FlyAdmin && Player) { return true; }
    return orig_IsPoseFallingHigh(Player);
}

bool (*get_InFallingState)(void *Player);
bool _get_InFallingState(void *Player){
	if (FlyTelePLUS && Player) { return false; }
    if (AutoTeleportMark && Player) { return true; }
	if (FlyTester && Player) { return false; }
    return get_InFallingState(Player);
}

bool NofallDmg = false;
bool (*orig_IsIgnoreHighFalling)(void *Player);
bool hook_IsIgnoreHighFalling(void *Player){
    //if (FlyHover && Player) { return true; }
	if (FlyTelePLUS && Player) { return true; }
	if (FlyTester && Player) { return true; }
	if (NofallDmg && Player) { return true; }
     return orig_IsIgnoreHighFalling(Player);
}

bool GroundHack = false;
bool (*isGroundedEngine)(void *instance);
bool _isGroundedEngine(void *instance) {
    //if (instance && FlyHover){ return true; }
	if (instance && GroundHack){ return true; }
    return isGroundedEngine(instance);
}

void (*orig_ShowDamage)(void* thiz, int damage, void* colliderT, void* p, int shieldDamage, int weaponID, float delay);
void hook_ShowDamage(void* thiz, int damage, void* colliderT, void* p, int shieldDamage, int weaponID, float delay) {
    if (AimkillStart2 && EnableFunction) {
        return;
    }
	if (Hidedamage && EnableFunction){
		return;
	}
    orig_ShowDamage(thiz, damage, colliderT, p, shieldDamage, weaponID, delay);
}

bool (*orig_get_ShowDamageNum)(void* thiz);
bool hook_get_ShowDamageNum(void* thiz) {
    if (AimkillStart2 && EnableFunction) {
        return false;
    }
	if (Hidedamage && EnableFunction) {
        return false;
    }
    return orig_get_ShowDamageNum(thiz);
}

bool speedvelo = false;
bool FlyTireV2 = false; 
float FlyTireSpeed = 25.0f; 

Vector3 (*old_GetTargetDirection)(void* instance);
Vector3 GetTargetDirection_Hook(void* instance) {
    Vector3 res = old_GetTargetDirection(instance);
    if (FlyTireV2 && EnableFunction) {
        res.y = FlyTireSpeed; 
    }
    return res;
}

Vector3 (*old_GetFallingDirection)(void* instance);
Vector3 GetFallingDirection_Hook(void* instance) {
    Vector3 res = old_GetFallingDirection(instance);
    if (FlyTireV2 && EnableFunction) {
        res.y = FlyTireSpeed; 
    }
    return res;
}

bool TestVelo = false;
float FlyTestVelo = 0.0f;
Vector3 (*old_GetVelocity)(void* instance);
Vector3 GetVelocity_Hook(void* instance) {
    Vector3 res = old_GetVelocity(instance);
    if (FlyTireV2 && EnableFunction) {
        res.y = FlyTireSpeed; 
    }
	if (TestVelo) {
        res.y = FlyTestVelo; 
    }
	if (speedvelo && EnableFunction) {
        res.x *= 2.0f;
        res.y *= 2.0f;
        res.z *= 2.0f;
    }
    return res;
}


bool fastswitch = false;
bool (*get_InSwapWeaponCD)(void* thiz);
bool _get_InSwapWeaponCD(void* thiz) {
  if (fastswitch && EnableFunction) {
        return false;
    }
    return get_InSwapWeaponCD(thiz);
}
bool Norecoil = false;
float (*ScatterRate)(void *instance);
float _ScatterRate(void *instance) {
    void* localPlayer = GetLocalPlayer(Curent_Match());
    if (instance != nullptr && instance == localPlayer) {
        if (Norecoil && EnableFunction) {
            return 0.1f; 
        }
    }
    return ScatterRate(instance);
}
bool InfiniteRange = false;
float (*orig_get_Range)(void *instance);
float hook_get_Range(void *instance) {
    if (instance != NULL) {
        if (InfiniteRange && EnableFunction) {
            return 9999.0f;
        }
    }
    return orig_get_Range(instance);
}

bool EnableFov = false;
float Fovset = 90.f;
float defaultFov = 60.0f; // ค่าเริ่มต้นเผื่อไว้

void (*orig_SetFOV)(void*, float);
void hook_SetFOV(void* cam, float fov){
    defaultFov = fov; 
    if (EnableFov && EnableFunction) {
        fov = Fovset;
    } else {
        fov = defaultFov;
    }
    orig_SetFOV(cam, fov);
}

bool UnlockTraining = false;

int (*get_UserLevel)(void *player);
int _get_UserLevel(void *player) {
    if (player != nullptr) {
        if(UnlockTraining && EnableFunction){
            return 12;
        }
    }
    return get_UserLevel(player);
}

bool highfps = false;

bool (*orig_IsHighFPS120Open)();
bool hook_IsHighFPS120Open() {
    if (highfps && EnableFunction) {
        return true;
    }
    return orig_IsHighFPS120Open();
}

bool (*orig_IsHighFPS144Open)();
bool hook_IsHighFPS144Open() {
    if (highfps && EnableFunction) {
        return true;
    }
    return orig_IsHighFPS144Open();
}

monoString* (*old_get_NickName)(void* instance);
monoString* hook_get_NickName(void* instance) {
    if (instance == nullptr) {
    }
    void* match = Curent_Match();
    if (match != nullptr) {
        void* local = GetLocalPlayer(match);           
        if (instance == local) {
            static monoString* cachedName = nullptr;
            if (cachedName == nullptr) {
                cachedName = il2cpp_string_new(OBFUSCATE("[FF0000]TELEGRAM [000000]: [FFFFFF] t.me/BloodShotTH"));
            }
            return cachedName;
        }
    }
    return old_get_NickName(instance);
}
#include "messageclass.h"

uint32_t hooked_messageID = 0;
void* hooked_msg_ptr = nullptr; 
uint8_t hooked_sendOption = 0;
bool hooked_cacheMsgAnyWay = false;
bool is_data_received = false;

bool enableClassLog = false; 
bool AfkNobot = false;

//====================================
bool (*GameFacadeSend)(uint32_t messageID, void* msg, uint8_t sendOption, bool cacheMsgAnyWay) = nullptr;
bool _GameFacadeSend(uint32_t messageID, void* msg, uint8_t sendOption, bool cacheMsgAnyWay) {
    if (EnableGhost) {
        return false;
    }
	if (LogSend) {
		hooked_messageID = messageID;
    	hooked_msg_ptr = msg;
    	hooked_sendOption = sendOption;
    	hooked_cacheMsgAnyWay = cacheMsgAnyWay;
    	is_data_received = true;
	}
    if (enableClassLog && msg != nullptr && (uintptr_t)msg > 0x1000) {
        static std::unordered_set<uint32_t> logged_ids;
        if (logged_ids.find(messageID) == logged_ids.end()) {
            
            void* il2cpp_class = *(void**)((uintptr_t)msg + 0x0); 
            if ((uintptr_t)il2cpp_class > 0x1000) {            
                
                #if defined(__arm__) 
                    const char* class_name = *(const char**)((uintptr_t)il2cpp_class + 0x8); 
                #else 
                    const char* class_name = *(const char**)((uintptr_t)il2cpp_class + 0x10); 
                #endif

                if (class_name != nullptr) {
                    std::ofstream log_file("/storage/emulated/0/Android/data/com.dts.freefireth/GameLogs.txt", std::ios::app);
                    if (log_file.is_open()) {
                        const char* action_name = GetMessageName(messageID);
                        log_file << "ID: " << messageID << " [" << action_name << "] -> Class: " << class_name << "\n";
                        log_file.close(); 
                        logged_ids.insert(messageID);
                    }
                }
            }
        }
    }
    return GameFacadeSend(messageID, msg, sendOption, cacheMsgAnyWay);
}

bool FlyJumpingToggle = false; // ตัวแปรเปิด-ปิดฟังก์ชัน

bool (*orig_get_IsInSAPFlyJumping)(void *instance);
bool hook_get_IsInSAPFlyJumping(void *instance) {
    if (FlyJumpingToggle && instance != nullptr) {
        return true; // บังคับให้เป็น true เมื่อเปิดใช้งาน
    }
    return orig_get_IsInSAPFlyJumping(instance);
}

bool (*O_NeedTransition) (void* _this);
bool hook_NeedTransition(void* _this) {
    if (AutoTeleportMark) TeleMark = true;
	if (AutoTeleportPlayer) TPPlayer = true;
    return O_NeedTransition(_this);
}


bool IndiaLogin = false;

int (*orig_ClientVersion)(void* _this);
int hook_ClientVersion(void* _this) {
    void* CurrentMatch = Curent_Match();
    if (!CurrentMatch) {
        void* LocalPlayer = GetLocalPlayer(CurrentMatch);
        if (!LocalPlayer) {    
            if (IndiaLogin) {
                return 3;
            }
        }
    }
    return orig_ClientVersion(_this);
}
