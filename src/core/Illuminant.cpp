/***********************************************************************
|*  Copyright (C) 2024 Arlen Avakian
|*
|*  This file is part of Kolourmatica.
|*
|*  Kolourmatica is free software: you can redistribute it and/or modify
|*  it under the terms of the GNU General Public License as published by
|*  the Free Software Foundation, either version 3 of the License, or
|*  (at your option) any later version.
|*
|*  Kolourmatica is distributed in the hope that it will be useful,
|*  but WITHOUT ANY WARRANTY; without even the implied warranty of
|*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
|*  GNU General Public License for more details.
|*
|*  You should have received a copy of the GNU General Public License
|*  along with Kolourmatica.  If not, see <http://www.gnu.org/licenses/>.
|*
|************************************************************************/


#include <Illuminant.hpp>


auto km::A_1931_2()     -> Illuminant { return internal::as_illuminant(0.44758, 0.40745); }
auto km::A_1964_10()    -> Illuminant { return internal::as_illuminant(0.45117, 0.40594); }

auto km::B_1931_2()     -> Illuminant { return internal::as_illuminant(0.34842, 0.35161); }
auto km::B_1964_10()    -> Illuminant { return internal::as_illuminant(0.3498, 0.3527); }

auto km::C_1931_2()     -> Illuminant { return internal::as_illuminant(0.31006, 0.31616); }
auto km::C_1964_10()    -> Illuminant { return internal::as_illuminant(0.31039, 0.31905); }

auto km::D50_1931_2()   -> Illuminant { return internal::as_illuminant(0.34567, 0.3585); }
auto km::D50_1964_10()  -> Illuminant { return internal::as_illuminant(0.34773, 0.35952); }

auto km::D55_1931_2()   -> Illuminant { return internal::as_illuminant(0.33242, 0.34743); }
auto km::D55_1964_10()  -> Illuminant { return internal::as_illuminant(0.33411, 0.34877); }

auto km::D65_1931_2()   -> Illuminant { return internal::as_illuminant(0.31271, 0.32902); }
auto km::D65_1964_10()  -> Illuminant { return internal::as_illuminant(0.31382, 0.331); }

auto km::D75_1931_2()   -> Illuminant { return internal::as_illuminant(0.29902, 0.31485); }
auto km::D75_1964_10()  -> Illuminant { return internal::as_illuminant(0.29968, 0.3174); }

auto km::E_1931_2()     -> Illuminant { return internal::as_illuminant(f64(1) / f64(3), f64(1) / f64(3)); }
auto km::E_1964_10()    -> Illuminant { return internal::as_illuminant(f64(1) / f64(3), f64(1) / f64(3)); }

auto km::F1_1931_2()    -> Illuminant { return internal::as_illuminant(0.3131, 0.33727); }
auto km::F1_1964_10()   -> Illuminant { return internal::as_illuminant(0.31811, 0.33559); }

auto km::F2_1931_2()    -> Illuminant { return internal::as_illuminant(0.37208, 0.37529); }
auto km::F2_1964_10()   -> Illuminant { return internal::as_illuminant(0.37925, 0.36733); }

auto km::F3_1931_2()    -> Illuminant { return internal::as_illuminant(0.4091, 0.3943); }
auto km::F3_1964_10()   -> Illuminant { return internal::as_illuminant(0.41761, 0.38324); }

auto km::F4_1931_2()    -> Illuminant { return internal::as_illuminant(0.44018, 0.40329); }
auto km::F4_1964_10()   -> Illuminant { return internal::as_illuminant(0.4492, 0.39074); }

auto km::F5_1931_2()    -> Illuminant { return internal::as_illuminant(0.31379, 0.34531); }
auto km::F5_1964_10()   -> Illuminant { return internal::as_illuminant(0.31975, 0.34246); }

auto km::F6_1931_2()    -> Illuminant { return internal::as_illuminant(0.3779, 0.38835); }
auto km::F6_1964_10()   -> Illuminant { return internal::as_illuminant(0.3866, 0.37847); }

auto km::F7_1931_2()    -> Illuminant { return internal::as_illuminant(0.31292, 0.32933); }
auto km::F7_1964_10()   -> Illuminant { return internal::as_illuminant(0.31569, 0.3296); }

auto km::F8_1931_2()    -> Illuminant { return internal::as_illuminant(0.34588, 0.35875); }
auto km::F8_1964_10()   -> Illuminant { return internal::as_illuminant(0.34902, 0.35939); }

auto km::F9_1931_2()    -> Illuminant { return internal::as_illuminant(0.37417, 0.37281); }
auto km::F9_1964_10()   -> Illuminant { return internal::as_illuminant(0.37829, 0.37045); }

auto km::F10_1931_2()   -> Illuminant { return internal::as_illuminant(0.34609, 0.35986); }
auto km::F10_1964_10()  -> Illuminant { return internal::as_illuminant(0.3509, 0.35444); }

auto km::F11_1931_2()   -> Illuminant { return internal::as_illuminant(0.38052, 0.37713); }
auto km::F11_1964_10()  -> Illuminant { return internal::as_illuminant(0.38541, 0.37123); }

auto km::F12_1931_2()   -> Illuminant { return internal::as_illuminant(0.43695, 0.40441); }
auto km::F12_1964_10()  -> Illuminant { return internal::as_illuminant(0.44256, 0.39717); }

auto km::getRefWhite(enums::RefWhite rw, enums::RefWhiteFov fov) -> Illuminant
{
    const auto invalid_rw = Illuminant{nan_tri()};

    switch (fov) {
        case enums::RefWhiteFov::_1931_2:
            switch (rw) {
                using enums::RefWhite;
                case RefWhite::A:   return A_1931_2();
                case RefWhite::B:   return B_1931_2();
                case RefWhite::C:   return C_1931_2();
                case RefWhite::D50: return D50_1931_2();
                case RefWhite::D55: return D55_1931_2();
                case RefWhite::D65: return D65_1931_2();
                case RefWhite::D75: return D75_1931_2();
                case RefWhite::E:   return E_1931_2();
                case RefWhite::F1:  return F1_1931_2();
                case RefWhite::F2:  return F2_1931_2();
                case RefWhite::F3:  return F3_1931_2();
                case RefWhite::F4:  return F4_1931_2();
                case RefWhite::F5:  return F5_1931_2();
                case RefWhite::F6:  return F6_1931_2();
                case RefWhite::F7:  return F7_1931_2();
                case RefWhite::F8:  return F8_1931_2();
                case RefWhite::F9:  return F9_1931_2();
                case RefWhite::F10: return F10_1931_2();
                case RefWhite::F11: return F11_1931_2();
                case RefWhite::F12: return F12_1931_2();
                default:
                    return invalid_rw;
            }
        case enums::RefWhiteFov::_1964_10:
            switch (rw) {
                using enums::RefWhite;
                case RefWhite::A:   return A_1964_10();
                case RefWhite::B:   return B_1964_10();
                case RefWhite::C:   return C_1964_10();
                case RefWhite::D50: return D50_1964_10();
                case RefWhite::D55: return D55_1964_10();
                case RefWhite::D65: return D65_1964_10();
                case RefWhite::D75: return D75_1964_10();
                case RefWhite::E:   return E_1964_10();
                case RefWhite::F1:  return F1_1964_10();
                case RefWhite::F2:  return F2_1964_10();
                case RefWhite::F3:  return F3_1964_10();
                case RefWhite::F4:  return F4_1964_10();
                case RefWhite::F5:  return F5_1964_10();
                case RefWhite::F6:  return F6_1964_10();
                case RefWhite::F7:  return F7_1964_10();
                case RefWhite::F8:  return F8_1964_10();
                case RefWhite::F9:  return F9_1964_10();
                case RefWhite::F10: return F10_1964_10();
                case RefWhite::F11: return F11_1964_10();
                case RefWhite::F12: return F12_1964_10();
                default:
                    return invalid_rw;
            }
        default: return invalid_rw;
    }
}
